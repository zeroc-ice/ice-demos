// Copyright (c) ZeroC, Inc.

import Foundation
import Ice
import Glacier2

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
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

// Create a proxy to the Greeter object in the server behind the Glacier2 router. Typically, the client cannot connect
// directly to this server because it's on an unreachable network.
var greeter = try makeProxy(
    communicator: communicator, proxyString: "greeter:tcp -h localhost -p 4061",
    type: GreeterPrx.self)

// Configure the proxy to route requests using the Glacier2 router.
greeter = greeter.ice_router(router)

// Send a request to the remote object and get the response.
let greeting = try await greeter.greet(NSUserName())
print(greeting)

// Send a second request to observe the effect in the Glacier2 router log.
let secondGreeting = try await greeter.greet("alice")
print(secondGreeting)
