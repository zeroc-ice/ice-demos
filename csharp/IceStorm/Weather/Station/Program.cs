// Copyright (c) ZeroC, Inc.

using ClearSky;
using System.Diagnostics;

// Create an Ice communicator. We'll use this communicator to create proxies, manage  outgoing connections, and create
// an object adapter.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants. This object adapter
// listens on an OS-assigned TCP port, on all interfaces.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("StationAdapter", "tcp");

// Register the ConsolePrinter servant with the adapter, and get a proxy to the new object. We use a UUID for the
// identity because these subscribers (weather stations) are transient: when a subscriber shuts down, we want IceStorm
// to remove the subscription.
WeatherStationPrx weatherStation = WeatherStationPrxHelper.uncheckedCast(
    adapter.addWithUUID(new Station.ConsolePrinter()));

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

// Register our weather station with the topic.
// subscribeAndGetPublisherAsync returns a publisher proxy that we don't need here.
_ = await topic.subscribeAndGetPublisherAsync(theQoS: [], weatherStation);

// Start dispatching requests.
adapter.activate();
Console.WriteLine("Listening...");

// Shut down the communicator when the user presses Ctrl+C.
Console.CancelKeyPress += (sender, eventArgs) =>
{
    eventArgs.Cancel = true; // don't terminate the process
    Console.WriteLine("Caught Ctrl+C, shutting down...");
    communicator.shutdown(); // starts shutting down
};

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted;
