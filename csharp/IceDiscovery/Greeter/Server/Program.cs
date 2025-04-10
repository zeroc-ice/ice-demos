// Copyright (c) ZeroC, Inc.

// Register the IceDiscovery plugin. The plugin will be loaded during communicator initialization.
IceDiscovery.Util.registerIceDiscovery(loadOnInitialize: true);

// Create an Ice communicator. We'll use this communicator to create an object adapter.
// The IceDiscovery plugin is created and initialized by initialize, and sets the default locator on the new
// communicator.
await using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Get the communicator's properties. We'll use this object to set the properties of our object adapter.
var properties = communicator.getProperties();

// Configure the object adapter GreeterAdapter. It must be an indirect object adapter (i.e., with an AdapterId
// property); otherwise, the IceDiscovery plugin can't make it discoverable by IceDiscovery clients.
properties.setProperty("GreeterAdapter.AdapterId", "greeterAdapterId");

// Configure the GreeterAdapter to listen on TCP with an OS-assigned port. We don't need a fixed port since the clients
// discover this object adapter.
properties.setProperty("GreeterAdapter.Endpoints", "tcp");

// Create an object adapter that listens for incoming requests and dispatches them to servants. "GreeterAdapter" is a
// key into the properties set above.
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
