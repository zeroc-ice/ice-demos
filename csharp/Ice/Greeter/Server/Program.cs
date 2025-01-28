// Copyright (c) ZeroC, Inc.

using GreeterServer;

// Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants.
var adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

// Register the Chatbot servant with the adapter.
adapter.add(new Chatbot(), Ice.Util.stringToIdentity("greeter"));

// Start dispatching requests.
adapter.activate();

// Wait until the user presses Ctrl+C.
await CancelKeyPressed;
