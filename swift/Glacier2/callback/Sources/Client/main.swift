// Copyright (c) ZeroC, Inc.

import DateExtension
import Foundation
import Ice
import Glacier2

// Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and to
// create an object adapter.
var args = CommandLine.arguments
let communicator = try Ice.initialize(&args)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is derived from
// the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
let router = try makeProxy(
    communicator: communicator, proxyString: "Glacier2/router:tcp -h localhost -p 4063",
    type: Glacier2.RouterPrx.self)

// Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
// username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime of the
// session is the same as the lifetime of the connection.
let session = try await router.createSession(userId: NSUserName(), password: "password")

// The proxy returned by createSession is nil because we did not configure a SessionManager on the Glacier2 router.
precondition(session == nil)

// Obtain a category string from the router. We need to use this category for the identity of server->client callbacks
// invoked through the Glacier2 router.
let clientCategory = try await router.getCategoryForClient()

// Create an object adapter with no name and no configuration, but with our router proxy. This object adapter is a
// "bidirectional" object adapter, like the one created by the Ice/bidir client application. It does not listen on any
// port and it does not need to be activated.
let adapter = try communicator.createObjectAdapterWithRouter(name: "", rtr: router)

// Register the MockAlarmClock servant with the adapter. It uses the category retrieved from the router. You can verify
// the Ring callback is never delivered if you provide a different category.
let mockAlarmClock = MockAlarmClock()
let alarmClock = try uncheckedCast(
    prx: adapter.add(servant: mockAlarmClock, id: Ice.Identity(name: "alarmClock", category: clientCategory)),
    type: AlarmClockPrx.self
)

// Create a proxy to the wake-up service behind the Glacier2 router. Typically, the client cannot connect directly to
// this server because it's on an unreachable network.
var wakeUpService = try makeProxy(
    communicator: communicator, proxyString: "wakeUpService:tcp -h localhost -p 4061",
    type: WakeUpServicePrx.self)

// Configure the proxy to route requests using the Glacier2 router.
wakeUpService = wakeUpService.ice_router(router)

// Schedule a wake-up call in 5 seconds. This call establishes the connection to the server; incoming requests over this
// connection are handled by the adapter.
let wakeUpTime = Date().addingTimeInterval(5)
try await wakeUpService.wakeMeUp(alarmClock: alarmClock, timeStamp: wakeUpTime.timeStamp)
print("Wake-up call scheduled, falling asleep...")

// Wait until the "stop" button is pressed on the mock alarm clock.
await mockAlarmClock.stopPressed()
print("Stop button pressed, exiting...")
