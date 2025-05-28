// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
var args = CommandLine.arguments
let communicator = try Ice.initialize(&args)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create an object adapter with no name and no configuration. This object adapter does not need to be activated.
let adapter = try communicator.createObjectAdapter("")

// Sets this object adapter as the default object adapter on the communicator.
communicator.setDefaultObjectAdapter(adapter)

// Register the MockAlarmClock servant with the adapter. The wake up service knows we use identity "alarmClock".
let mockAlarmClock = MockAlarmClock()
try adapter.add(servant: mockAlarmClock, id: Ice.Identity(name: "alarmClock"))

// Create a proxy to the wake-up service.
let wakeUpService = try makeProxy(
    communicator: communicator, proxyString: "wakeUpService:tcp -h localhost -p 4061",
    type: WakeUpServicePrx.self)

// Schedule a wake-up call in 5 seconds. This call establishes the connection to the server; incoming requests over this
// connection are handled by the communicator's default object adapter.
let wakeUpTime = Date().addingTimeInterval(5)
try await wakeUpService.wakeMeUp(wakeUpTime.timeStamp)

print("Wake-up call scheduled, falling asleep...")

// Wait until the "stop" button is pressed on the mock alarm clock.
await mockAlarmClock.stopPressed()
print("Stop button pressed, exiting...")
