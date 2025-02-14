// Copyright (c) ZeroC, Inc.

// Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

// Create a CancellationTokenSource to cancel outstanding dispatches after the user presses Ctrl+C.
using var cts = new CancellationTokenSource();

// Register two instances of Chatbot - a regular greater and a slow greeter.
adapter.add(new Server.Chatbot(TimeSpan.Zero, CancellationToken.None), Ice.Util.stringToIdentity("greeter"));
adapter.add(new Server.Chatbot(TimeSpan.FromSeconds(60), cts.Token), Ice.Util.stringToIdentity("slowGreeter"));

// Start dispatching requests.
adapter.activate();
Console.WriteLine("Listening on port 4061...");

// Wait until the user presses Ctrl+C.
await CancelKeyPressed;
Console.WriteLine("Caught Ctrl+C, exiting...");

// Cancel outstanding dispatches "stuck" in the slow greeter.
cts.Cancel();
