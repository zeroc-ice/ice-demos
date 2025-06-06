// Copyright (c) ZeroC, Inc.

import Ice

// CtrlCHandler is a helper struct that handles Ctrl+C and similar signals. It must be constructed at the beginning
// of the program, before creating an Ice communicator or starting any thread.
let ctrlCHandler = CtrlCHandler()

// Configure the communicator to load the IceDiscovery plug-in during initialization. This plug-in installs a default
// locator on the communicator.
var args = CommandLine.arguments
let properties = try Ice.createProperties(&args)
properties.setProperty(key: "Ice.Plugin.IceDiscovery", value: "1")

// Configure the object adapter GreeterAdapter. It must be an indirect object adapter (i.e., with an AdapterId
// property); otherwise, the IceDiscovery plugin can't make it discoverable by IceDiscovery clients.
properties.setProperty(key: "GreeterAdapter.AdapterId", value: "greeterAdapterId")

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

// Register the Chatbot servant with the adapter.
try adapter.add(servant: Chatbot(), id: Ice.Identity(name: "greeter"))

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
