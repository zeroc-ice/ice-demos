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

// Create a Greeter proxy. If you run the server on a different computer, replace localhost in the string below with
// the server's hostname or IP address.
let greeter = try makeProxy(
    communicator: communicator, proxyString: "greeter:tcp -h localhost -p 4061",
    type: GreeterPrx.self)

// We hardcode the tokens in this demo, for simplicity. A real application would obtain the token from a secure source.
let validToken = "iced tea"

// Send a request to the remote object with an invalid token in the request context.
do {
    let unexpected = try await greeter.greet(NSUserName(), context: ["token": "pineapple"])
    print("Received unexpected greeting: \(unexpected)")
} catch is DispatchException {
    // Expected with an invalid (or missing) token. See AuthorizationMiddleware.
}

// Send a request with the correct token in the request context.

let greeting = try await greeter.greet(NSUserName(), context: ["token": validToken])
print(greeting)
