// Copyright (c) ZeroC, Inc.

// Create Ice properties from the contents of the config.server file in the current working directory.
var properties = new Ice.Properties();
properties.load("config.server");

// Create an Ice communicator. We'll use this communicator to create an object adapter.
// The communicator gets its properties from initData.properties; Ice.* properties and other reserved properties set in
// args override these properties.
await using Ice.Communicator communicator = Ice.Util.initialize(
    ref args,
    new Ice.InitializationData { properties = properties });

// Create an object adapter that listens for incoming requests and dispatches them to servants.
// This adapter is configured through the GreeterAdapter.* properties in config.server.
Ice.ObjectAdapter adapter = communicator.createObjectAdapter("GreeterAdapter");

// Register the Chatbot servant with the adapter.
adapter.add(new Server.Chatbot(), new Ice.Identity { name = "greeter" });

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
