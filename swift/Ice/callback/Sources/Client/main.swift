// Copyright (c) ZeroC, Inc.

import DateExtension
import Foundation
import Ice

// Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and to
// create an object adapter.
var args = CommandLine.arguments
let communicator = try Ice.initialize(&args)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create an object adapter that listens for incoming requests and dispatches them to servants.
// Since we don't specify a port, the OS will choose an ephemeral port. This allows multiple client applications to
// run concurrently on the same host.
let adapter = try communicator.createObjectAdapterWithEndpoints(name: "AlarmClockAdapter", endpoints: "tcp");

// Register the MockAlarmClock servant with the adapter, and get an alarm clock proxy.
let mockAlarmClock = MockAlarmClock()
let alarmClock = try uncheckedCast(
    prx: adapter.add(servant: mockAlarmClock, id: Ice.Identity(name: "alarmClock")),
    type: AlarmClockPrx.self
)

// Start dispatching requests.
try adapter.activate()
print("Listening on ephemeral port...")

// Create a proxy to the wake-up service.
let wakeUpService = try makeProxy(
    communicator: communicator, proxyString: "wakeUpService:tcp -h localhost -p 4061",
    type: WakeUpServicePrx.self)

// Schedule a wake-up call in 5 seconds.
let wakeUpTime = Date().addingTimeInterval(5)
try await wakeUpService.wakeMeUp(alarmClock: alarmClock, timeStamp: wakeUpTime.timeStamp)
print("Wake-up call scheduled, falling asleep...")

// Wait until the "stop" button is pressed on the mock alarm clock.
await mockAlarmClock.stopPressed()
print("Stop button pressed, exiting...")
