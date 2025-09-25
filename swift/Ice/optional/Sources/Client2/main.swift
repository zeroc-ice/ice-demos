// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
let communicator = try Ice.initialize(CommandLine.arguments)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create a proxy to the weather station.
let weatherStation = try makeProxy(
    communicator: communicator, proxyString: "weatherStation:tcp -h localhost -p 4061",
    type: WeatherStationPrx.self)

// Create a new AtmosphericConditions object with random values.
let reading = AtmosphericConditions(
    temperature: Double(Int.random(in: 190...230)) / 10.0,
    humidity: Double(Int.random(in: 450...550)) / 10.0,
    pressure: Double(Int.random(in: 10_000...10_500)) / 10.0
)

// Report this reading to the weather station.
try await weatherStation.report(sensorId: "sensor v2", reading: reading)

print("sensor v2: sent reading to weather station")
