// Copyright (c) ZeroC, Inc.

// Create an Ice communicator. We'll use this communicator to create an object adapter.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("ForwarderAdapter", "tcp -p 10000");

// Create a target proxy template, with a dummy identity.
Ice.ObjectPrx targetTemplate = Ice.ObjectPrxHelper.createProxy(communicator, "dummy:tcp -h localhost -p 4061");

// Register the Forwarder servant as default servant with the object adapter. The empty category means this default
// servant receives requests to all Ice objects.
adapter.addDefaultServant(new ForwardingServer.Forwarder(targetTemplate), category: "");

// Start dispatching requests.
adapter.activate();
Console.WriteLine("Listening on port 10000...");

// Shut down the communicator when the user presses Ctrl+C.
Console.CancelKeyPress += (sender, eventArgs) =>
{
    eventArgs.Cancel = true; // don't terminate the process
    Console.WriteLine("Caught Ctrl+C, shutting down...");
    communicator.shutdown(); // starts shutting down
};

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted;
