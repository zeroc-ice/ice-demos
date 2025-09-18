// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// Load the contents of the config.client file into a Properties object.
let configFileProperties = Ice.createProperties()
try configFileProperties.load("config.client")

// Create a Properties object from the command line arguments and the config file properties; Ice.* properties and
// other reserved properties set in args augment or override the config file properties.
var args = CommandLine.arguments
let properties = try Ice.createProperties(&args, defaults: configFileProperties)

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// The communicator gets its properties from the properties object.
var initData = Ice.InitializationData()
initData.properties = properties
let communicator = try Ice.initialize(initData)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
// It's nil if the property is not set.
guard let objectPrx = try communicator.propertyToProxy("Greeter.Proxy") else {
    fatalError("Greeter.Proxy property is not set in config.client")
}

let greeter = uncheckedCast(prx: objectPrx, type: GreeterPrx.self)

// Send a request to the remote object and get the response.
let greeting = try await greeter.greet(NSUserName())

print(greeting)
