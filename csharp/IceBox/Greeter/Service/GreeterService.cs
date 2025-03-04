// Copyright (c) ZeroC, Inc.

using VisitorCenter;

namespace Service;

/// <summary>An IceBox service that provides an implementation of the Greeter interface.</summary>
internal class GreeterService : IceBox.Service
{
    /// <inheritdoc/>
    public void start(string name, Ice.Communicator communicator, string[] args)
    {
        // Create an object adapter that listens for incoming requests and dispatches them to servants.
        Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

        // Register the Chatbot servant with the adapter.
        adapter.add(new Chatbot("Hal"), Ice.Util.stringToIdentity("greeter"));

        // Start dispatching requests.
        adapter.activate();
        Console.WriteLine("Listening on port 4061...");
    }

    /// <inheritdoc/>
    public void stop()
    {
        // Nothing to do.
    }
}
