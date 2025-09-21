// Copyright (c) ZeroC, Inc.

package com.example.ice.config.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.Properties;
import com.zeroc.Ice.Util;

class Client {
    public static void main(String[] args) {
        // Load the contents of the config.client file into a Properties object.
        var configFileProperties = new Properties();
        configFileProperties.load("config.client");

        // Create a Properties object from the command line arguments and the config file properties; Ice.* properties
        // and other reserved properties set in args augment or override the config file properties.
        var properties = new Properties(args, configFileProperties);

        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        // The communicator gets its properties from the properties object.
        var initData = new InitializationData();
        initData.properties = properties;
        try (Communicator communicator = Util.initialize(initData)) {
            // We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
            GreeterPrx greeter = GreeterPrx.uncheckedCast(communicator.propertyToProxy("Greeter.Proxy"));

            // Send a request to the remote object and get the response.
            String greeting = greeter.greet(System.getProperty("user.name"));
            System.out.println(greeting);
        }
    }
}
