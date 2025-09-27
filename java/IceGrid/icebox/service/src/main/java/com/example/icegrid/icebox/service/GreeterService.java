// Copyright (c) ZeroC, Inc.

package com.example.icegrid.icebox.service;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Util;
import com.zeroc.Ice.Properties;
import com.zeroc.IceBox.Service;

/**
 * An IceBox service that provides an implementation of the Greeter interface.
 */
public class GreeterService implements Service {
    // The object adapter created by this service.
    private ObjectAdapter _adapter;

    @Override
    public void start(String name, Communicator communicator, String[] args) {
        // Create an object adapter. It's configured by the GreeterAdapter.* properties in the IceGrid-generated
        // config file.
        _adapter = communicator.createObjectAdapter("GreeterAdapter");

        // Retrieve the greeter name and greeter identity from the IceGrid-generated config file.
        Properties properties = communicator.getProperties();
        String greeterName = properties.getProperty("Ice.ProgramName");
        Identity greeterIdentity = Util.stringToIdentity(properties.getProperty("Greeter.Identity"));

        // Register the Chatbot servant with the adapter.
        _adapter.add(new Chatbot(greeterName), greeterIdentity);

        // Start dispatching requests.
        _adapter.activate();
        System.out.println(greeterName + " is listening...");
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
