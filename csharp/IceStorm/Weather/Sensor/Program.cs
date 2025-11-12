// Copyright (c) ZeroC, Inc.

// Slice module ClearSky in WeatherStation.ice maps to C# namespace ClearSky.
using ClearSky;
using System.Diagnostics;
using System.Globalization;
using System.Security.Cryptography; // for RandomNumberGenerator

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using var communicator = new Ice.Communicator(ref args);

// Create a proxy to the IceStorm topic manager.
IceStorm.TopicManagerPrx topicManager = IceStorm.TopicManagerPrxHelper.createProxy(
    communicator,
    "ClearSky/TopicManager:tcp -p 4061 -h localhost");

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

// Create a WeatherStation proxy using the publisher proxy of the topic.
Ice.ObjectPrx? publisher = await topic.getPublisherAsync();
Debug.Assert(publisher is not null); // The proxy returned by getPublisher is never null.
WeatherStationPrx weatherStation = WeatherStationPrxHelper.uncheckedCast(publisher);

// The proxy returned by IceStorm is a two-way proxy. We can convert it into a oneway proxy if we don't need
// acknowledgments from IceStorm.
Debug.Assert(weatherStation.ice_isTwoway());

// Generate a sensor ID for this weather sensor.
string sensorId = $"sensor-{RandomNumberGenerator.GetHexString(4)}";

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
        Temperature = RandomNumberGenerator.GetInt32(190, 230) / 10.0,
        Humidity = RandomNumberGenerator.GetInt32(450, 550) / 10.0,
    };

    try
    {
        // Send the reading to the weather station(s).
        string timeStamp = DateTime.Now.ToString("T", CultureInfo.CurrentCulture);
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
