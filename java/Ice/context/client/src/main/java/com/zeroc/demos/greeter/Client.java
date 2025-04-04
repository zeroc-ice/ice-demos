// Copyright (c) ZeroC, Inc.

package com.zeroc.demos.greeter;

import com.zeroc.demos.VisitorCenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.Properties;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.Map;

class Client {
    public static void main(String[] args) {
        // Set the Ice.ImplicitContext property to "Shared" before calling Ice.Util.initialize.
        // This is only necessary for the implicit context API (see below).
        var initData = new InitializationData();
        initData.properties = new Properties(args);
        initData.properties.setProperty("Ice.ImplicitContext", "Shared");

        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = com.zeroc.Ice.Util.initialize(initData)) {
            // Create a Greeter proxy. If you run the server on a different computer, replace localhost in the string
            // below with the server's hostname or IP address.
            GreeterPrx greeter = GreeterPrx.createProxy(communicator, "greeter:tcp -h localhost -p 4061");

            // Send a request to the remote object and get the response. We request a French greeting by setting the
            //  context parameter.
            String greeting = greeter.greet(System.getProperty("user.name"), Map.of("language", "fr"));
            System.out.println(greeting);

            // Do it again, this time by setting the context on the proxy.
            var greeterEs = GreeterPrx.uncheckedCast(greeter.ice_context(Map.of("language", "es")));
            greeting = greeterEs.greet("alice");
            System.out.println(greeting);

            // One more time, this time with an implicit context set on the communicator.
            communicator.getImplicitContext().put("language", "de");
            greeting = greeter.greet("bob");
            System.out.println(greeting);
        }
    }
}
