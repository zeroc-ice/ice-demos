// Copyright (c) ZeroC, Inc.

using Filesystem;

// Create an Ice communicator. We'll use this communicator to create an object adapter.
await using var communicator = new Ice.Communicator(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("Filesystem", "tcp -p 4061");

// Create the root directory servant (with name "/"), and add this servant to the adapter.
var root = new Server.MDirectory("/");
adapter.add(root, new Ice.Identity { name = "RootDir" });

// Create a file called "README", add this servant to the adapter,
// and add the corresponding proxy to the root directory.
var file = new Server.MFile("README");
file.WriteDirect(["This file system contains a collection of poetry."]);
root.AddChild(FilePrxHelper.uncheckedCast(adapter.addWithUUID(file)));

// Create a directory called "Coleridge", add this servant to the adapter,
// and add the corresponding proxy to the root directory.
var coleridge = new Server.MDirectory("Coleridge");
root.AddChild(DirectoryPrxHelper.uncheckedCast(adapter.addWithUUID(coleridge)));

// Create a file called "Kubla_Khan", add this servant to the adapter,
// and add the corresponding proxy to the Coleridge directory.
file = new Server.MFile("Kubla_Khan");
file.WriteDirect([
    "In Xanadu did Kubla Khan",
    "A stately pleasure-dome decree:",
    "Where Alph, the sacred river, ran",
    "Through caverns measureless to man",
    "Down to a sunless sea."
]);
coleridge.AddChild(FilePrxHelper.uncheckedCast(adapter.addWithUUID(file)));

// Start dispatching requests after registering all servants.
adapter.activate();
Console.WriteLine("Listening on port 4061...");

// Shut down the communicator when the user presses Ctrl+C.
Console.CancelKeyPress += (sender, eventArgs) =>
{
    eventArgs.Cancel = true; // don't terminate the process
    Console.WriteLine("Caught Ctrl+C, shutting down...");
    communicator.shutdown(); // starts shutting down
};

// Wait until the communicator is shut down.
// Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted;
