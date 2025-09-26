// Copyright (c) ZeroC, Inc.

package com.example.icediscovery.replication.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.Properties;
import com.zeroc.Ice.Util;
import com.zeroc.IceDiscovery.PluginFactory;

import java.util.Collections;

class Client {
    public static void main(String[] args) {
        // Configure the communicator to load the IceDiscovery plug-in during initialization. This plug-in installs a
        // default locator on the communicator.
        var initData = new InitializationData();
        initData.properties = new Properties(args);
        initData.pluginFactories = Collections.singletonList(new PluginFactory());

        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = Util.initialize(initData)) {
            // Create a proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no
            // addressing information, also known as a well-known proxy. It's resolved by the default locator installed
            // by the IceDiscovery plug-in.
            var greeter = GreeterPrx.createProxy(communicator, "greeter");

            // Send a request to the remote object and get the response.
            String greeting = greeter.greet(System.getProperty("user.name"));
            System.out.println(greeting);
        }
    }
}
