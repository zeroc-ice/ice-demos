// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
let communicator = try Ice.initialize(CommandLine.arguments)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// We create a Greeter proxy for a Greeter object in the Forwarding server (port 10000).
let greeter = try makeProxy(
    communicator: communicator, proxyString: "greeter:tcp -h localhost -p 10000",
    type: GreeterPrx.self)

// Send a request to the remote object and get the response.
let greeting = try await greeter.greet(NSUserName())

print(greeting)
