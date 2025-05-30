// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// Set the Ice.ImplicitContext property to "Shared" before creating the communicator.
// This is only necessary for the implicit context API (see below).
var args = CommandLine.arguments
let initData = Ice.InitializationData()
initData.properties = Ice.createProperties()
initData.properties.setProperty(key: "Ice.ImplicitContext", value: "Shared")

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
let communicator = try Ice.initialize(&args, initData)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create a Greeter proxy. If you run the server on a different computer, replace localhost in the string below with
// the server's hostname or IP address.
let greeter = try makeProxy(
    communicator: communicator, proxyString: "greeter:tcp -h localhost -p 4061",
    type: GreeterPrx.self)

// Send a request to the remote object and get the response. We request a French greeting by setting the context
// parameter.
var greeting = try await greeter.greet(
    NSUserName(),
    context: ["language": "fr"])

print(greeting)

// Do it again, this time by setting the context on the proxy.
let greeterEs = uncheckedCast(
    prx: greeter.ice_context(["language": "es"]),
    type: GreeterPrx.self)

greeting = try await greeterEs.greet("alice")
print(greeting)

// One more time, this time with an implicit context set on the communicator.
communicator.getImplicitContext()?.put(key: "language", value: "de")
greeting = try await greeter.greet("bob")
print(greeting)