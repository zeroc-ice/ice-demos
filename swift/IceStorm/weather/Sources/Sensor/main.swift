// Copyright (c) ZeroC, Inc.

import Foundation
import Ice
import IceStorm

// CtrlCHandler is a helper struct that handles Ctrl+C and similar signals. It must be constructed at the beginning
// of the program, before creating an Ice communicator or starting any thread.
let ctrlCHandler = CtrlCHandler()

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
let communicator = try Ice.initialize(CommandLine.arguments)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create a proxy to the IceStorm topic manager.
let topicManager = try makeProxy(
    communicator: communicator, proxyString: "ClearSky/TopicManager:tcp -h localhost -p 4061",
    type: IceStorm.TopicManagerPrx.self)

var topic: IceStorm.TopicPrx
let topicName = "weather"

// Retrieve a proxy to the "weather" topic: we first create a topic with the given name (in case we are the first),
// and then retrieve the proxy if the topic was already created.
// The proxy returned by create and retrieve is never nil.
do {
    topic = try await topicManager.create(topicName)!
} catch is IceStorm.TopicExists {
    topic = try await topicManager.retrieve(topicName)!
}

// Create a WeatherStation proxy using the publisher proxy of the topic.
let publisher = try await topic.getPublisher()
let weatherStation = uncheckedCast(prx: publisher!, type: WeatherStationPrx.self)

// The proxy returned by IceStorm is a two-way proxy. We can convert it into a oneway proxy if we don't need
// acknowledgments from IceStorm.
precondition(weatherStation.ice_isTwoway())

// Generate a sensor ID for this weather sensor.
let sensorId = "sensor-\(UUID().uuidString.prefix(4))"

// Print a message indicating the sensor is reporting.
print("\(sensorId) reporting. Press Ctrl+C to stop...")

let dateFormatter = DateFormatter()
dateFormatter.dateFormat = "H:mm:ss.SSS"

// Send a reading every second until cancelled.
let task = Task {
    while true {
        do {
            // Create a new AtmosphericConditions object with random values.
            let reading = AtmosphericConditions(
                temperature: Double(Int.random(in: 190...230)) / 10.0,
                humidity: Double(Int.random(in: 450...550)) / 10.0
            )
            let timeStamp = dateFormatter.string(from: Date())

            try await weatherStation.report(sensorId: sensorId, timeStamp: timeStamp, reading: reading)
        } catch let error as Ice.LocalException {
            print("report failed with: \(error.message)")
        }
        try await Task.sleep(for: .seconds(1))
    }
}

// Cancel the background task when the user presses Ctrl+C.
ctrlCHandler.setCallback { signal in
    print("Caught signal \(signal), exiting...")
    task.cancel()
}

// Wait until the user presses Ctrl+C.
do {
    try await task.value
} catch is CancellationError {
    // expected
}
