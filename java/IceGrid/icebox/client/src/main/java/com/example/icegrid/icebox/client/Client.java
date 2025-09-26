// Copyright (c) ZeroC, Inc.

package com.example.icegrid.icebox.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.LocatorPrx;
import com.zeroc.Ice.Util;

class Client {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = Util.initialize(args)) {

            // Set the default locator of the new communicator. It's the address of the Locator hosted by our IceGrid
            // registry. You can also set this proxy with the Ice.Default.Locator property.
            communicator.setDefaultLocator(
                LocatorPrx.createProxy(communicator, "IceGrid/Locator:tcp -h localhost -p 4061"));

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
