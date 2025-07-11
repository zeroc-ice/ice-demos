// Copyright (c) ZeroC, Inc.

import Ice

// CtrlCHandler is a helper struct that handles Ctrl+C and similar signals. It must be constructed at the beginning
// of the program, before creating an Ice communicator or starting any thread.
let ctrlCHandler = CtrlCHandler()

// Create an Ice communicator. We'll use this communicator to create an object adapter.
var args = CommandLine.arguments
let communicator = try Ice.initialize(&args)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create an object adapter that listens for incoming requests and dispatches them to servants.
let adapter = try communicator.createObjectAdapterWithEndpoints(
    name: "StationAdapter", endpoints: "tcp -p 4061")

// Register the ConsolePrinter servant with the adapter.
try adapter.add(servant: ConsolePrinter(), id: Ice.Identity(name: "weatherStation"))

// Start dispatching requests.
try adapter.activate()
print("Listening on port 4061...")

// Shutdown the communicator when the user presses Ctrl+C.
ctrlCHandler.setCallback { signal in
    print("Caught signal \(signal), shutting down...")
    communicator.shutdown()
}

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted()
