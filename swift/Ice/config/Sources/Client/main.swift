// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections. The
// communicator gets its configuration properties from file config.client in the client's current working directory.
// The communicator initialization also parses the command-line options to find and set additional properties.
var args = CommandLine.arguments
let communicator = try Ice.initialize(args: &args, configFile: "config.client")

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
