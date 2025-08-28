// Copyright (c) ZeroC, Inc.

import Ice

// CtrlCHandler is a helper struct that handles Ctrl+C and similar signals.
// It must be constructed at the beginning of the program,
// before creating an Ice communicator or starting any thread.
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
    name: "Filesystem", endpoints: "tcp -p 4061")

// Create the root directory servant (with name "/"), and add this servant to the adapter.
let root = MDirectory(name: "/")
try adapter.add(servant: root, id: Ice.Identity(name: "RootDir"))

// Create a file called "README", add this servant to the adapter,
// and add the corresponding proxy to the root directory.
let readme = MFile(name: "README")
await readme.writeDirect(text: ["This file system contains a collection of poetry."])
try await root.addChild(uncheckedCast(prx: adapter.addWithUUID(readme), type: FilePrx.self))

// Create a directory called "Coleridge", add this servant to the adapter,
// and add the corresponding proxy to the root directory.
let coleridge = MDirectory(name: "Coleridge")
try await root.addChild(uncheckedCast(prx: adapter.addWithUUID(coleridge), type: DirectoryPrx.self))

// Create a file called "Kubla_Khan", add this servant to the adapter,
// and add the corresponding proxy to the Coleridge directory.
let kublaKhan = MFile(name: "Kubla_Khan")
await kublaKhan.writeDirect(text: [
    "In Xanadu did Kubla Khan",
    "A stately pleasure-dome decree:",
    "Where Alph, the sacred river, ran",
    "Through caverns measureless to man",
    "Down to a sunless sea.",
])
try await coleridge.addChild(uncheckedCast(prx: adapter.addWithUUID(kublaKhan), type: FilePrx.self))

// Start dispatching requests after registering all servants.
try adapter.activate()
print("Listening on port 4061...")

// Shutdown the communicator when the user presses Ctrl+C.
ctrlCHandler.setCallback { signal in
    print("Caught signal \(signal), shutting down...")
    communicator.shutdown()
}

// Wait until the communicator is shut down.
// Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted()
