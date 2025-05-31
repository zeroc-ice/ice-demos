// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
// of the program, before creating an Ice communicator or starting any thread.
let ctrlCHandler = CtrlCHandler()

// Generate a unique name for the adapter ID and the greeter name.
let uuid = UUID().uuidString

// Configure the communicator to load the IceDiscovery plug-in during initialization. This plug-in installs a default
// locator on the communicator.
var args = CommandLine.arguments
let properties = try Ice.createProperties(&args)
properties.setProperty(key: "Ice.Plugin.IceDiscovery", value: "1")

// Configure the object adapter GreeterAdapter. It must be an indirect object adapter (i.e., with an AdapterId
// property); otherwise, the IceDiscovery plugin can't make it discoverable by IceDiscovery clients.
// We also set the ReplicaGroupId property to "greeterPool" to enable replication.
properties.setProperty(key: "GreeterAdapter.AdapterId", value: "greeter-\(uuid)")
properties.setProperty(key: "GreeterAdapter.ReplicaGroupId", value: "greeterPool")

// Configure the GreeterAdapter to listen on TCP with an OS-assigned port. We don't need a fixed port since the clients
// discover this object adapter.
properties.setProperty(key: "GreeterAdapter.Endpoints", value: "tcp")

var initData = Ice.InitializationData()
initData.properties = properties

// Create an Ice communicator. We'll use this communicator to create an object adapter.
let communicator = try Ice.initialize(initData)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create an object adapter that listens for incoming requests and dispatches them to servants. "GreeterAdapter" is a
// key into the configuration properties set above.
let adapter = try communicator.createObjectAdapter("GreeterAdapter")

// Register the Chatbot servant with the adapter. Here, well-known object "greeter" is replicated across all server
// instances.
let greeterName = String(uuid.suffix(4))
try adapter.add(servant: Chatbot(greeterName: greeterName), id: Ice.Identity(name: "greeter"))

// Start dispatching requests. This method also registers the object adapter with the IceDiscovery plugin.
try adapter.activate()
print("Listening...")

// Shutdown the communicator when the user presses Ctrl+C.
ctrlCHandler.setCallback { signal in
    print("Caught signal \(signal), shutting down...")
    communicator.shutdown()
}

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted()
