// Copyright (c) ZeroC, Inc.

package com.zeroc.demos.greeter;

import com.zeroc.demos.greeter.Chatbot;

class Server {
    public static void main(String[] args)
    {
        // Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
        try (com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args)) {
            Runtime.getRuntime().addShutdownHook(new Thread(() -> {
                communicator.shutdown();
            }));

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

            // Register the Chatbot servant with the adapter.
            adapter.add(new Chatbot(), com.zeroc.Ice.Util.stringToIdentity("greeter"));

            // Start dispatching requests.
            adapter.activate();
            System.out.println("Listening on port 4061...");

            // Wait until the user presses Ctrl+C.
            communicator.waitForShutdown();
            System.out.println("Caught Ctrl+C, exiting...");
        }
    }
}
