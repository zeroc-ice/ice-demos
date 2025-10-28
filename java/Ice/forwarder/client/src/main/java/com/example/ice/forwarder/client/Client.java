// Copyright (c) ZeroC, Inc.

package com.example.ice.forwarder.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;

class Client {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = new Communicator(args)) {
            // We create a Greeter proxy for a Greeter object in the Forwarding server (port 10000).
            GreeterPrx greeter = GreeterPrx.createProxy(communicator, "greeter:tcp -h localhost -p 10000");

            // Send a request to the remote object and get the response.
            String greeting = greeter.greet(System.getProperty("user.name"));
            System.out.println(greeting);
        }
    }
}
