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

// Set the default locator of the new communicator. It's the address of the Locator hosted by our IceGrid registry.
// You can also set this proxy with the Ice.Default.Locator property.
let locator = try makeProxy(
    communicator: communicator, proxyString: "IceGrid/Locator:tcp -h localhost -p 4061",
    type: Ice.LocatorPrx.self)
communicator.setDefaultLocator(locator)

// Create a proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no addressing
// information, also known as a well-known proxy. It's specified by the <object> element in the IceGrid XML file.
// The IceGrid registry resolves this well-known proxy and returns the actual address (endpoint) of the server to this
// client.
let greeter = try makeProxy(
    communicator: communicator, proxyString: "greeter",
    type: GreeterPrx.self)

// Send a request to the remote object and get the response.
let greeting = try await greeter.greet(NSUserName())
print(greeting)

// Send another request to the remote object. With the default configuration we use for this client, this request
// reuses the connection and reaches the same server, even when we have multiple replicated servers.
let secondGreeting = try await greeter.greet("alice")
print(secondGreeting)
