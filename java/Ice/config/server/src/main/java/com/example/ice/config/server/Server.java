// Copyright (c) ZeroC, Inc.

package com.example.ice.config.server;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Properties;
import com.zeroc.Ice.Util;

class Server {
    public static void main(String[] args) {
        // Create Ice properties from the contents of the config.server file in the current working directory.
        var initData = new InitializationData();
        initData.properties = new Properties();
        initData.properties.load("config.server");

        // Create an Ice communicator. We'll use this communicator to create an object adapter.
        // The communicator gets its properties from initData.properties; Ice.* properties and other reserved properties
        // set in args override these properties.
        try (Communicator communicator = Util.initialize(args, initData)) {

            // Register a shutdown hook that calls communicator.shutdown() when the user shuts down the server with
            // Ctrl+C or similar. The shutdown hook thread also waits until the main thread completes its cleanup.
            shutdownCommunicatorOnCtrlC(communicator, Thread.currentThread());

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            ObjectAdapter adapter = communicator.createObjectAdapter("GreeterAdapter");

            // Register the Chatbot servant with the adapter.
            adapter.add(new Chatbot(), new Identity("greeter", ""));

            // Start dispatching requests.
            adapter.activate();
            System.out.println("Listening on port 4061...");

            // Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
            communicator.waitForShutdown();
        }
    }

    private static void shutdownCommunicatorOnCtrlC(Communicator communicator, Thread mainThread) {
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("Caught Ctrl+C, shutting down...");
            communicator.shutdown();

            // Wait until the main thread completes.
            try {
                mainThread.join();
            } catch (InterruptedException e) {
                // No code interrupts the shutdown hook thread in this program.
                assert false;
            }
        }));
    }
}
