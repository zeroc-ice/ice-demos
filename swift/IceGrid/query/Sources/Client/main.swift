// Copyright (c) ZeroC, Inc.

import Foundation
import Ice
import IceGrid

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

// Create a proxy to the Query object hosted by the IceGrid registry. "IceGrid/Query" a well-known proxy, without
// addressing information.
let query = try makeProxy(
    communicator: communicator, proxyString: "IceGrid/Query",
    type: IceGrid.QueryPrx.self)

// Look up an object with type ::VisitorCenter::Greeter.
let greeterTypeId = GreeterPrx.ice_staticId() // ::VisitorCenter::Greeter
let proxy = try await query.findObjectByType(greeterTypeId)

if proxy == nil {
    print("The IceGrid registry doesn't know any object with type '\(greeterTypeId)'.")
} else {
    // Cast the object proxy to a Greeter proxy.
    let greeter = try makeProxy(proxy: proxy!, type: GreeterPrx.self)

    // Send a request to the remote object and get the response.
    let greeting = try await greeter.greet(NSUserName())
    print(greeting)

    // Send another request to the remote object. With the default configuration we use for this client, this request
    // reuses the connection and reaches the same server.
    let secondGreeting = try await greeter.greet("alice")
    print(secondGreeting)
}
