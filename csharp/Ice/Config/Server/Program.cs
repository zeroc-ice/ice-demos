// Copyright (c) ZeroC, Inc.

// Create an Ice communicator to initialize the Ice runtime. The communicator gets its configuration properties from
// file config.server, in the server's current working directory. The communicator initialization also parses the
// command-line options to find and set additional properties.
using Ice.Communicator communicator = Ice.Util.initialize(ref args, "config.server");

// Create an object adapter that listens for incoming requests and dispatches them to servants.
// This adapter is configured through the GreeterAdapter.* properties in config.server.
Ice.ObjectAdapter adapter = communicator.createObjectAdapter("GreeterAdapter");

// Register the Chatbot servant with the adapter.
adapter.add(new Server.Chatbot(), Ice.Util.stringToIdentity("greeter"));

// Start dispatching requests.
adapter.activate();

// Ice.Trace.Network=1 or greater shows on which interface(s) and port(s) the server is listening.

// Wait until the user presses Ctrl+C.
await CancelKeyPressed;
Console.WriteLine("Caught Ctrl+C, exiting...");
