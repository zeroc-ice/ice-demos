// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// Configure the communicator to load the IceLocatorDiscovery plug-in during initialization. This plug-in will discover
// the locator (IceGrid registry in this demo) to use. As a result, we don't need to configure the default locator on
// this communicator.
var args = CommandLine.arguments
let properties = try Ice.createProperties(&args)
properties.setProperty(key: "Ice.Plugin.IceLocatorDiscovery", value: "1")

var initData = Ice.InitializationData()
initData.properties = properties

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
let communicator = try Ice.initialize(initData)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

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
