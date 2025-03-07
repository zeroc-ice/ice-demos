// Copyright (c) ZeroC, Inc.

// Slice module ClearSky in WeatherStation.ice maps to C# namespace ClearSky.
using ClearSky;
using System.Diagnostics;

// Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create a proxy to the IceStorm topic manager.
IceStorm.TopicManagerPrx topicManager = IceStorm.TopicManagerPrxHelper.createProxy(
    communicator,
    "ClearSky/TopicManager:tcp -p 4061");

// Retrieve a proxy to the "weather" topic: we first create a topic with the given name (in case we are the first),
// and then retrieve the proxy if the topic was already created.
IceStorm.TopicPrx? topic;
string topicName = "weather";
try
{
    topic = await topicManager.createAsync(topicName);
}
catch (IceStorm.TopicExists)
{
    topic = await topicManager.retrieveAsync(topicName);
}

// The proxy returned by createAsync and retrieveAsync is never null.
Debug.Assert(topic is not null);

// Create a Weather station proxy using the publisher proxy of the topic. The proxy returned by getPublisher is
// never null.
WeatherStationPrx weatherStation = WeatherStationPrxHelper.uncheckedCast(topic.getPublisher()!);

// The proxy returned by IceStorm is a two-way proxy. We can convert it into a oneway proxy if we don't need
// acknowledgments from IceStorm.
Debug.Assert(weatherStation.ice_isTwoway());

var rand = new Random(); // pseudo-random number generator

// Generate a sensor ID for this weather sensor.
string sensorId = $"sensor-{rand.Next(1000)}";

// Send a reading every second to the weather station(s) via IceStorm. We keep sending until the user presses Ctrl+C.
Console.WriteLine($"{sensorId} reporting. Press Ctrl+C to stop...");

using var cts = new CancellationTokenSource();
var cancellationToken = cts.Token;
Console.CancelKeyPress += (sender, eventArgs) =>
{
    eventArgs.Cancel = true;
    cts.Cancel();
};

while (!cancellationToken.IsCancellationRequested)
{
    // Create a new AtmosphericConditions object with random values.
    var reading = new AtmosphericConditions
    {
        Temperature = rand.Next(190, 230) / 10.0,
        Humidity = rand.Next(450, 550) / 10.0,
    };

    try
    {
        // Send the reading to the weather station(s).
        string timeStamp = DateTime.Now.ToString("T");
        await weatherStation.ReportAsync(sensorId, timeStamp, reading, cancel: cancellationToken);

        // Wait for one second before sending the next reading.
        await Task.Delay(TimeSpan.FromSeconds(1), cancellationToken);
    }
    catch (OperationCanceledException)
    {
        // The user pressed Ctrl+C, we exit the while loop.
        break;
    }
}

Console.WriteLine("Exiting...");
