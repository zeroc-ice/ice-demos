// Copyright (c) ZeroC, Inc.

package com.example.ice.greeter.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Util;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;

class Client {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = Util.initialize(args, "config.client")) {
            // We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
            GreeterPrx greeter = GreeterPrx.uncheckedCast(communicator.propertyToProxy("Greeter.Proxy"));

            // Send a request to the remote object and get the response.
            CompletableFuture<String> futureGreeting = greeter.greetAsync("alice");

            try {
                String greeting = futureGreeting.get();
                System.out.println(greeting);
            } catch (InterruptedException e) {
                assert false; // We don't interrupt this thread.
            } catch (ExecutionException e) {
                System.out.println("Could not get greeting: " + e.getMessage());
            }
        }
    }
}
