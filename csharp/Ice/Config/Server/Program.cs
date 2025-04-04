// Copyright (c) ZeroC, Inc.

// Create an Ice communicator. We'll use this communicator to create an object adapter. The communicator gets its
// configuration properties from file config.server, in the server's current working directory. The communicator
// initialization also parses the command-line options to find and set additional properties.
await using Ice.Communicator communicator = Ice.Util.initialize(ref args, "config.server");

// Create an object adapter that listens for incoming requests and dispatches them to servants.
// This adapter is configured through the GreeterAdapter.* properties in config.server.
Ice.ObjectAdapter adapter = communicator.createObjectAdapter("GreeterAdapter");

// Register the Chatbot servant with the adapter.
adapter.add(new Server.Chatbot(), Ice.Util.stringToIdentity("greeter"));

// Start dispatching requests.
adapter.activate();

// Ice.Trace.Network=1 or greater shows on which interface(s) and port(s) the server is listening.

// Shut down the communicator when the user presses Ctrl+C.
Console.CancelKeyPress += (sender, eventArgs) =>
{
    eventArgs.Cancel = true; // don't terminate the process
    Console.WriteLine("Caught Ctrl+C, shutting down...");
    communicator.shutdown(); // starts shutting down
};

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted;
