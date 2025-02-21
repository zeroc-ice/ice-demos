// Copyright (c) ZeroC, Inc.

package com.zeroc.greeter;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Util;
import com.zeroc.greeter.VisitorCenter.GreeterPrx;
import java.util.concurrent.CompletableFuture;

class Client {
    public static void main(String[] args)
    {
        // Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args))
        {
            // GreeterPrx is a class generated by the Slice compiler. We create a proxy from a communicator and a "stringified
            // proxy" with the address of the target object.
            // If you run the server on a different computer, replace localhost in the string below with the server's hostname
            // or IP address.
            GreeterPrx greeter = GreeterPrx.createProxy(communicator, "greeter:tcp -h localhost -p 4061");

            // Send a request to the remote object and get the response.
            String greeting = greeter.greet(System.getProperty("user.name"));
            System.out.println(greeting);

            // Send another request to the remote object, this time with greetAsync. greetAsync returns a future
            // immediately.
            CompletableFuture<String> result = greeter.greetAsync("alice");

            try {
                System.out.println(result.get());
            } catch(Exception ex) {
                ex.printStackTrace();
            }
        }
    }
}
