// Copyright (c) ZeroC, Inc.

import Ice

// CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
// of the program, before creating an Ice communicator or starting any thread.
let ctrlCHandler = CtrlCHandler()

// Create an Ice communicator. We'll use this communicator to create an object adapter. The communicator gets its
// configuration properties from file config.server, in the server's current working directory. The communicator
// initialization also parses the command-line options to find and set additional properties.
var args = CommandLine.arguments
let communicator = try Ice.initialize(args: &args, configFile: "config.server")

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create an object adapter that listens for incoming requests and dispatches them to servants.
// This adapter is configured through the GreeterAdapter.* properties in config.server.
let adapter = try communicator.createObjectAdapter("GreeterAdapter")

// Register the Chatbot servant with the adapter.
try adapter.add(servant: Chatbot(), id: Ice.Identity(name: "greeter"))

// Start dispatching requests.
try adapter.activate()

// Ice.Trace.Network=1 or greater shows on which interface(s) and port(s) the server is listening.

// Shutdown the communicator when the user presses Ctrl+C.
ctrlCHandler.setCallback { signal in
    print("Caught signal \(signal), shutting down...")
    communicator.shutdown()
}

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted()
