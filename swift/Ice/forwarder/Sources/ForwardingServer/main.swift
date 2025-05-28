// Copyright (c) ZeroC, Inc.

import Ice

// CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
// of the program, before creating an Ice communicator or starting any thread.
let ctrlCHandler = CtrlCHandler()

// Create an Ice communicator. We'll use this communicator to create an object adapter, and to create proxies and
// manage outgoing connections.
var args = CommandLine.arguments
let communicator = try Ice.initialize(&args)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create an object adapter that listens for incoming requests and dispatches them to servants.
let adapter = try communicator.createObjectAdapterWithEndpoints(
    name: "ForwarderAdapter", endpoints: "tcp -p 10000")

// Create a target proxy template, with a dummy identity.
let targetTemplate = try makeProxy(communicator: communicator,
                                   proxyString: "dummy:tcp -h localhost -p 4061",
                                   type: Ice.ObjectPrx.self)

// Register the Forwarder servant as default servant with the object adapter. The empty category means this default
// servant receives requests to all Ice objects.
try adapter.addDefaultServant(servant: Forwarder(targetTemplate), category: "")

// Start dispatching requests.
try adapter.activate()
print("Listening on port 10000...")

// Shutdown the communicator when the user presses Ctrl+C.
ctrlCHandler.setCallback { signal in
    print("Caught signal \(signal), shutting down...")
    communicator.shutdown()
}

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted()
