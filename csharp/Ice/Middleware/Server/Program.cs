// Copyright (c) ZeroC, Inc.

// Create an Ice communicator to initialize the Ice runtime.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

// Install the Authorization middleware in the object adapter.
adapter.use(next => new Server.AuthorizationMiddleware(next, validToken: "iced tea"));

// Register the Chatbot servant with the adapter.
adapter.add(new Server.Chatbot(), Ice.Util.stringToIdentity("greeter"));

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
