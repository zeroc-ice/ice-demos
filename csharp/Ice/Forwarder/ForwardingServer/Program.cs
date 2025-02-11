// Copyright (c) ZeroC, Inc.

using ForwardingServer;

// Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("ForwarderAdapter", "tcp -p 10000");

// Create a target proxy template, with a dummy identity.
Ice.ObjectPrx targetTemplate = Ice.ObjectPrxHelper.createProxy(communicator, "dummy:tcp -h localhost -p 4061");

// Register the Forwarder servant as default servant with the object adapter. The empty category means this default
// servant receives requests to all Ice objects.
adapter.addDefaultServant(new Forwarder(targetTemplate), category: "");

// Start dispatching requests.
adapter.activate();
Console.WriteLine("Listening on port 10000...");

// Wait until the user presses Ctrl+C.
await CancelKeyPressed;
Console.WriteLine("Caught Ctrl+C, exiting...");
