// Copyright (c) ZeroC, Inc.

// Configure the communicator to load the IceDiscovery plug-in during initialization. This plug-in installs a default
// locator on the communicator.
var initData = new Ice.InitializationData
{
    properties = new Ice.Properties(ref args),
    pluginFactories = [new IceDiscovery.PluginFactory()]
};

// Configure the object adapter GreeterAdapter. It must be an indirect object adapter (i.e., with an AdapterId
// property); otherwise, the IceDiscovery plug-in can't make it discoverable by IceDiscovery clients.
initData.properties.setProperty("GreeterAdapter.AdapterId", "greeterAdapterId");

// Configure the GreeterAdapter to listen on TCP with an OS-assigned port. We don't need a fixed port since the clients
// discover this object adapter.
initData.properties.setProperty("GreeterAdapter.Endpoints", "tcp");

// Create an Ice communicator. We'll use this communicator to create an object adapter.
await using var communicator = new Ice.Communicator(initData);

// Create an object adapter that listens for incoming requests and dispatches them to servants. "GreeterAdapter" is a
// key into the configuration properties set above.
Ice.ObjectAdapter adapter = communicator.createObjectAdapter("GreeterAdapter");

// Register the Chatbot servant with the adapter.
adapter.add(new Server.Chatbot(), new Ice.Identity { name = "greeter" });

// Start dispatching requests. This method also registers the object adapter with the IceDiscovery plug-in.
adapter.activate();
Console.WriteLine("Listening...");

// Shut down the communicator when the user presses Ctrl+C.
Console.CancelKeyPress += (sender, eventArgs) =>
{
    eventArgs.Cancel = true; // don't terminate the process
    Console.WriteLine("Caught Ctrl+C, shutting down...");
    communicator.shutdown(); // starts shutting down
};

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted;
