// Copyright (c) ZeroC, Inc.

// Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

// Register the Chatbot and ChatbotAdmin servants with the adapter.
using var chatbot = new Server.Chatbot();
adapter.add(chatbot, Ice.Util.stringToIdentity("greeter"));
adapter.addFacet(new Server.ChatbotAdmin(chatbot), Ice.Util.stringToIdentity("greeter"), "admin");

// Start dispatching requests.
adapter.activate();
Console.WriteLine("Listening on port 4061...");

// Wait until the user presses Ctrl+C.
await CancelKeyPressed;
Console.WriteLine("Caught Ctrl+C, exiting...");
