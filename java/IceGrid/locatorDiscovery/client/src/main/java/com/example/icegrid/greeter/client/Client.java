// Copyright (c) ZeroC, Inc.

package com.example.icegrid.greeter.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.Properties;
import com.zeroc.IceLocatorDiscovery.PluginFactory;

import java.util.Collections;

class Client {
    public static void main(String[] args) {
        // Configure the communicator to load the IceDiscovery plug-in during initialization. This plug-in installs a
        // default locator on the communicator.
        var initData = new InitializationData();
        initData.pluginFactories = Collections.singletonList(new PluginFactory());

        // Parse command-line arguments into properties.
        initData.properties = new Properties(args);

        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = new Communicator(initData)) {
            // Create a proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no
            // addressing information, also known as a well-known proxy. It's specified by the <object> element in the
            // IceGrid XML file. The IceGrid registry resolves this well-known proxy and returns the actual address
            // (endpoint) of the server to this client.
            var greeter = GreeterPrx.createProxy(communicator, "greeter");

            // Send a request to the remote object and get the response.
            String greeting = greeter.greet(System.getProperty("user.name"));
            System.out.println(greeting);

            // Send another request to the remote object. With the default configuration we use for this client, this
            // request reuses the connection and reaches the same server, even when we have multiple replicated servers.
            greeting = greeter.greet("alice");
            System.out.println(greeting);
        }
    }
}
