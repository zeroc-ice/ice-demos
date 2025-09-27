// Copyright (c) ZeroC, Inc.

package com.example.icegrid.query.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.LocatorPrx;
import com.zeroc.Ice.ObjectPrx;
import com.zeroc.Ice.Util;
import com.zeroc.IceGrid.QueryPrx;

class Client {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = Util.initialize(args)) {

            // Set the default locator of the new communicator. It's the address of the Locator hosted by our IceGrid
            // registry. You can also set this proxy with the Ice.Default.Locator property.
            communicator.setDefaultLocator(
                LocatorPrx.createProxy(communicator, "IceGrid/Locator:tcp -h localhost -p 4061"));

            // Create a proxy to the Query object hosted by the IceGrid registry. "IceGrid/Query" a well-known proxy,
            // without addressing information.
            QueryPrx query = QueryPrx.createProxy(communicator, "IceGrid/Query");

            // Look up an object with type ::VisitorCenter::Greeter.
            String greeterTypeId = GreeterPrx.ice_staticId(); // ::VisitorCenter::Greeter
            ObjectPrx proxy = query.findObjectByType(greeterTypeId);

            if (proxy == null) {
                System.out.println(
                    String.format("The IceGrid registry doesn't know any object with type '%s'.", greeterTypeId));
            } else {
                // Cast the object proxy to a Greeter proxy.
                GreeterPrx greeter = GreeterPrx.uncheckedCast(proxy);

                // Send a request to the remote object and get the response.
                String greeting = greeter.greet(System.getProperty("user.name"));
                System.out.println(greeting);

                // Send another request to the remote object. With the default configuration we use for this client,
                // this request reuses the connection and reaches the same server, even when we have multiple
                // replicated servers.
                greeting = greeter.greet("alice");
                System.out.println(greeting);
            }
        }
    }
}
