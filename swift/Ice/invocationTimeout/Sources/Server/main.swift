// Copyright (c) ZeroC, Inc.

import Ice

// CtrlCHandler is a helper struct that handles Ctrl+C and similar signals. It must be constructed at the beginning
// of the program, before creating an Ice communicator or starting any thread.
let ctrlCHandler = CtrlCHandler()

// Create an Ice communicator. We'll use this communicator to create an object adapter.
let communicator = try Ice.initialize(CommandLine.arguments)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create an object adapter that listens for incoming requests and dispatches them to servants.
let adapter = try communicator.createObjectAdapterWithEndpoints(
    name: "GreeterAdapter", endpoints: "tcp -p 4061")

// Register two instances of Chatbot - a regular greater and a slow greeter.
try adapter.add(servant: Chatbot(), id: Ice.Identity(name: "greeter"))
let slowGreeter = Chatbot(delay: .seconds(60))
try adapter.add(servant: slowGreeter, id: Ice.Identity(name: "slowGreeter"))

// Start dispatching requests.
try adapter.activate()
print("Listening on port 4061...")

// Shutdown the communicator when the user presses Ctrl+C.
ctrlCHandler.setCallback { signal in
    print("Caught signal \(signal), shutting down...")
    communicator.shutdown()

    // Cancel outstanding tasks in the slow greeter.
    Task {
        await slowGreeter.cancelOutstandingTasks()
    }
}

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted()
