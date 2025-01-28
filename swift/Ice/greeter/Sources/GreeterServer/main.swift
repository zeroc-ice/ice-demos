// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// Create an Ice communicator to initialize the Ice runtime.
let communicator = try Ice.initialize()

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Set up a signal handler to catch Ctrl-C and shut down the communicator.
signal(SIGINT) { _ in
    communicator.shutdown()
}

// Create an object adapter that listens for incoming requests and dispatches them to servants.
let adapter = try communicator.createObjectAdapterWithEndpoints(
    name: "GreeterAdapter", endpoints: "tcp -p 4061")

// Register the Chatbot servant with the adapter.
try adapter.add(servant: GreeterDisp(ChatBot()), id: Ice.stringToIdentity("greeter"))

// Start dispatching requests.
try adapter.activate()

// Wait until the communicator is shutdown, which happens when the signal handler is called.
communicator.waitForShutdown()
