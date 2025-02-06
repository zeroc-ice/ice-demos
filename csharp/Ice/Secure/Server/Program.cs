// Copyright (c) ZeroC, Inc.

using SecureServer;

// Create an Ice communicator to initialize the Ice runtime. The new communicator gets its properties from file
// "config.server".
using Ice.Communicator communicator = Ice.Util.initialize(ref args, "config.server");

// Create an object adapter that listens for incoming requests and dispatches them to servants.  We use the ssl
// transport for secure communications.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "ssl -p 4061");

// Register the Chatbot servant with the adapter.
adapter.add(new Chatbot(), Ice.Util.stringToIdentity("greeter"));

// Start dispatching requests.
adapter.activate();
Console.WriteLine("Listening on port 4061...");

// Wait until the user presses Ctrl+C.
await CancelKeyPressed;
Console.WriteLine("Caught Ctrl+C, exiting...");
