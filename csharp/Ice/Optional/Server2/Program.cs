// Copyright (c) ZeroC, Inc.

// Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("StationAdapter", "tcp -p 4061");

// Register the ConsolePrinter servant with the adapter.
_ = adapter.add(new Server.ConsolePrinter(), Ice.Util.stringToIdentity("weatherStation"));

// Start dispatching requests.
adapter.activate();
Console.WriteLine("Listening on port 4061...");

// Shut down the communicator when the user presses Ctrl+C.
Console.CancelKeyPress += (sender, eventArgs) =>
{
    eventArgs.Cancel = true; // don't terminate the process
    Console.WriteLine("Caught Ctrl+C, shutting down...");
    communicator.shutdown(); // starts shutting down
};

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted;
