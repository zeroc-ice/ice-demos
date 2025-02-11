// Copyright (c) ZeroC, Inc.

// Create an Ice communicator to initialize the Ice runtime.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("WakeUpAdapter", "tcp -p 4061");

// Register the SimpleWakeUpService servant with the adapter.
adapter.add(new Server.SimpleWakeUpService(), Ice.Util.stringToIdentity("wakeUpService"));

// Start dispatching requests.
adapter.activate();
Console.WriteLine("Listening on port 4061...");

// Wait until the user presses Ctrl+C.
await CancelKeyPressed;
Console.WriteLine("Caught Ctrl+C, exiting...");
