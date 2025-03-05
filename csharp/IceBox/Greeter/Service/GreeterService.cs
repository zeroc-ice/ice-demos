// Copyright (c) ZeroC, Inc.

using System.Diagnostics;
using VisitorCenter;

namespace Service;

/// <summary>An IceBox service that provides an implementation of the Greeter interface.</summary>
internal class GreeterService : IceBox.Service
{
    // The object adapter created by this service.
    private Ice.ObjectAdapter? _adapter;

    /// <inheritdoc/>
    public void start(string name, Ice.Communicator communicator, string[] args)
    {
        // Create an object adapter that listens for incoming requests and dispatches them to servants.
        Debug.Assert(_adapter is null);
        _adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

        // Register the Chatbot servant with the adapter.
        _adapter.add(new Chatbot("Syd"), Ice.Util.stringToIdentity("greeter"));

        // Start dispatching requests.
        _adapter.activate();
        Console.WriteLine("Listening on port 4061...");
    }

    /// <inheritdoc/>
    public void stop()
    {
        Console.WriteLine("Shutting down...");

        // We destroy the object adapter in stop() in case the service is started, stopped and then restarted again
        // programmatically or using an admin tool.
        // The general rule is stop should cleanup all resources created by start.
        Debug.Assert(_adapter is not null);
        _adapter.destroy();
        _adapter = null;
    }
}
