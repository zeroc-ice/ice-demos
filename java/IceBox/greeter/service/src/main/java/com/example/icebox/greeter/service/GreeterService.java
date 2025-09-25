// Copyright (c) ZeroC, Inc.

package com.example.icebox.greeter.service;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.IceBox.Service;

/**
 * An IceBox service that provides an implementation of the Greeter interface.
 */
public class GreeterService implements Service {

    // The object adapter created by this service.
    private ObjectAdapter _adapter;

    @Override
    public void start(String name, Communicator communicator, String[] args) {
        // Create an object adapter that listens for incoming requests and dispatches them to servants.
        _adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

        // Register the Chatbot servant with the adapter.
        _adapter.add(new Chatbot(), new Identity("greeter", ""));

        // Start dispatching requests.
        _adapter.activate();
        System.out.println("Listening on port 4061...");
    }

    @Override
    public void stop() {
        System.out.println("Shutting down...");

        // We destroy the object adapter in stop() in case the service is started, stopped and then restarted again
        // programmatically or using an admin tool.
        // The general rule is stop should cleanup all resources created by start.
        assert _adapter != null;
        _adapter.destroy();
        _adapter = null;
    }
}
