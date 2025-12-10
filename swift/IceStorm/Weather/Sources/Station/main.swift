// Copyright (c) ZeroC, Inc.

import Ice
import IceStorm

// CtrlCHandler is a helper struct that handles Ctrl+C and similar signals. It must be constructed at the beginning
// of the program, before creating an Ice communicator or starting any thread.
let ctrlCHandler = CtrlCHandler()

// Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and create
// an object adapter.
let communicator = try Ice.initialize(CommandLine.arguments)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create an object adapter that listens on an OS-assigned TCP port, on all interfaces.
let adapter = try communicator.createObjectAdapterWithEndpoints(name: "StationAdapter", endpoints: "tcp")

// Register the ConsolePrinter servant with the adapter, and get a proxy to the new object.
// We use a UUID for the identity because these subscribers (weather stations) are transient: if this program exits
// without unsubscribing its weather station, when it restarts, it logically creates a new weather station as opposed
// to re-incarnating the old one.
// See also the Retry Count QoS section in the IceStorm documentation.
let weatherStation = try uncheckedCast(prx: adapter.addWithUUID(ConsolePrinter()), type: WeatherStationPrx.self)

// Create a proxy to the IceStorm topic manager.
let topicManager = try makeProxy(
    communicator: communicator, proxyString: "ClearSky/TopicManager:tcp -h localhost -p 4061",
    type: IceStorm.TopicManagerPrx.self)

// Ask the topic manager to create or retrieve the "weather" topic and return the corresponding proxy.
// The proxy returned by createOrRetrieve is never nil.
let topicName = "weather"
let topic = try await topicManager.createOrRetrieve(topicName)!

// Start dispatching requests.
try adapter.activate()
print("Listening...")

// Register our weather station with the topic.
_ = try await topic.subscribeAndGetPublisher(theQoS: [:], subscriber: weatherStation)
print("Subscribed weather station to topic '\(topicName)'.")

// Shutdown the communicator when the user presses Ctrl+C.
ctrlCHandler.setCallback { signal in
    print("Caught signal \(signal), shutting down...")
    communicator.shutdown()
}

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted()

// Unsubscribe from the topic. The shutdown above only shuts down the object adapter. All client-side functionalities
// remain available until the communicator is destroyed.
try await topic.unsubscribe(weatherStation)
print("Unsubscribed weather station from topic '\(topicName)', exiting...")
