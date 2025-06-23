// Copyright (c) ZeroC, Inc.

package com.example.ice.config.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.Properties;
import com.zeroc.Ice.Util;

class Client {
    public static void main(String[] args) {
        // Create Ice properties from the contents of the config.client file in the current working directory.
        var initData = new InitializationData();
        initData.properties = new Properties();
        initData.properties.load("config.client");

        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        // The communicator gets its properties from initData.properties; Ice.* properties and other reserved properties
        // set in args override these properties.
        try (Communicator communicator = Util.initialize(args, initData)) {
            // We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
            GreeterPrx greeter = GreeterPrx.uncheckedCast(communicator.propertyToProxy("Greeter.Proxy"));

            // Send a request to the remote object and get the response.
            String greeting = greeter.greet(System.getProperty("user.name"));
            System.out.println(greeting);
        }
    }
}
