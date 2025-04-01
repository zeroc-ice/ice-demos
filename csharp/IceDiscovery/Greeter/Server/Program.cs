// Copyright (c) ZeroC, Inc.

// Configure the communicator to load the IceDiscovery plugin during initialization. This plugin installs a default
// locator on the communicator.
var initData = new Ice.InitializationData
{
    properties = new Ice.Properties(ref args)
};
initData.properties.setProperty("Ice.Plugin.Discovery", "IceDiscovery:IceDiscovery.PluginFactory");

// Configure the object adapter GreeterAdapter. It must be an indirect object adapter (i.e., with an AdapterId
// property); otherwise, the IceDiscovery plugin can't make it discoverable by IceDiscovery clients.
initData.properties.setProperty("GreeterAdapter.AdapterId", "greeterAdapterId");

// Configure the GreeterAdapter to listen on TCP with an OS-assigned port. We don't need a fixed port since the clients
// discover this object adapter.
initData.properties.setProperty("GreeterAdapter.Endpoints", "tcp");

// Create an Ice communicator. We'll use this communicator to create an object adapter.
using Ice.Communicator communicator = Ice.Util.initialize(initData);

// Create an object adapter that listens for incoming requests and dispatches them to servants. "GreeterAdapter" is a
// key into the configuration properties set above.
Ice.ObjectAdapter adapter = communicator.createObjectAdapter("GreeterAdapter");

// Register the Chatbot servant with the adapter.
adapter.add(new Server.Chatbot(), Ice.Util.stringToIdentity("greeter"));

// Start dispatching requests. This method also registers the object adapter with the IceDiscovery plugin.
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
