// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

// CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
// of the program, before creating an Ice communicator or starting any thread.
let ctrlCHandler = CtrlCHandler()

// Create an Ice communicator to initialize the Ice runtime.
let communicator = try Ice.initialize()

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create an object adapter that listens for incoming requests and dispatches them to servants.
let adapter = try communicator.createObjectAdapterWithEndpoints(
    name: "GreeterAdapter", endpoints: "tcp -p 4061")

// Register the Chatbot servant with the adapter.
try adapter.add(servant: GreeterDisp(Chatbot()), id: Ice.stringToIdentity("greeter"))

// Start dispatching requests.
try adapter.activate()

// Wait until the user presses Ctrl+C.
let signal = await ctrlCHandler.catchSignal()
print("Caught signal \(signal), exiting...")
