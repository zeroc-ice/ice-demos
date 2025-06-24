// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// Create Ice properties from the contents of the config.client file in the current working directory.
let properties = Ice.createProperties()
try properties.load("config.client")

var initData = Ice.InitializationData()
initData.properties = properties

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// The communicator gets its properties from initData.properties; Ice.* properties and other reserved properties set
// in args override these properties.
var args = CommandLine.arguments
let communicator = try Ice.initialize(&args, initData: initData)

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
