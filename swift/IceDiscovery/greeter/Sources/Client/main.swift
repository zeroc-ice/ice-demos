// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// Configure the communicator to load the IceDiscovery plug-in during initialization. This plug-in installs a default
// locator on the communicator.
let properties = try Ice.createProperties(CommandLine.arguments)
properties.setProperty(key: "Ice.Plugin.IceDiscovery", value: "1")

var initData = Ice.InitializationData()
initData.properties = properties

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
let communicator = try Ice.initialize(initData)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create a proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no addressing
// information, also known as a well-known proxy. It's resolved by the default locator installed by the IceDiscovery
// plugin.
let greeter = try makeProxy(
    communicator: communicator, proxyString: "greeter",
    type: GreeterPrx.self)

// Send a request to the remote object and get the response.
let greeting = try await greeter.greet(NSUserName())

print(greeting)
