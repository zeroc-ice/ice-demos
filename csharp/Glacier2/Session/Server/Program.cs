// Copyright (c) ZeroC, Inc.

// Create an Ice communicator. We'll use this communicator to create an object adapter.
await using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("PokeAdapter", "tcp -p 4061");

// Register the SessionManager servant with the adapter.
var sessionManager = new Server.SessionManager(adapter);
adapter.add(sessionManager, new Ice.Identity { name = "SessionManager" });

// Register the SharedPokePen servant with the adapter as a default servant for category "pen".
var sharedPokePen = new Server.SharedPokePen(new Server.MPokeStore(), sessionManager);
adapter.addDefaultServant(sharedPokePen, "pen");

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
