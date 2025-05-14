// Copyright (c) ZeroC, Inc.

package com.example.ice.config.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Util;

class Client {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections. 
        // The communicator gets its configuration properties from file config.client in the client's current working
        // directory. The communicator initialization also parses the command-line options to find and set additional
        // properties.
        try (Communicator communicator = Util.initialize(args, "config.client")) {
            // We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
            GreeterPrx greeter = GreeterPrx.uncheckedCast(communicator.propertyToProxy("Greeter.Proxy"));

            // Send a request to the remote object and get the response.
            String greeting = greeter.greet(System.getProperty("user.name"));
            System.out.println(greeting);
        }
    }
}
