// Copyright (c) ZeroC, Inc.

using System.Diagnostics;
using VisitorCenter;

namespace Service;

/// <summary>An IceBox service that provides an implementation of the Greeter interface.</summary>
internal class GreeterService : IceBox.Service
{
    // The object adapter created by this service.
    private Ice.ObjectAdapter? _adapter;

    private string _greeterName = "unknown";

    /// <inheritdoc/>
    public void start(string name, Ice.Communicator communicator, string[] args)
    {
        // Create an object adapter that listens for incoming requests and dispatches them to servants.
        // "GreeterAdapter" must match the name of the adapter in the IceGrid XML file.
        Debug.Assert(_adapter is null);
        _adapter = communicator.createObjectAdapter("GreeterAdapter");

        // Retrieve the greeter name and identity from the IceGrid-generated config file.
        Ice.Properties properties = communicator.getProperties();
        _greeterName = properties.getProperty("Greeter.Name");
        Ice.Identity greeterIdentity = Ice.Util.stringToIdentity(properties.getProperty("Greeter.Identity"));

        // Register the Chatbot servant with the adapter.
        _adapter.add(new Chatbot(_greeterName), greeterIdentity);

        // Start dispatching requests.
        _adapter.activate();
        Console.WriteLine($"{_greeterName} is listening...");
    }

    /// <inheritdoc/>
    public void stop()
    {
        Console.WriteLine($"{_greeterName} is shutting down...");

        // We destroy the object adapter in stop() in case the service is started, stopped and then restarted again
        // programmatically or using an admin tool.
        // The general rule is stop should cleanup all resources created by start.
        Debug.Assert(_adapter is not null);
        _adapter.destroy();
        _adapter = null;
    }
}
