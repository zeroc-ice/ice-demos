// Copyright (c) ZeroC, Inc.

package com.example.ice.endpointselection.server;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Properties;

class Server {
    public static void main(String[] args) {
        // Accept an optional port number as a plain argument (default 4061).
        int port = 4061;
        for (String arg : args) {
            if (!arg.startsWith("-")) {
                port = Integer.parseInt(arg);
                break;
            }
        }

        // Set up properties with the adapter endpoints and parse Ice.* args from the command line.
        var properties = new Properties(args);
        properties.setProperty("GreeterAdapter.Endpoints", "tcp -h localhost -p " + port);

        var initData = new InitializationData();
        initData.properties = properties;

        // Create an Ice communicator. We'll use this communicator to create an object adapter.
        try (Communicator communicator = new Communicator(initData)) {

            ObjectAdapter adapter = communicator.createObjectAdapter("GreeterAdapter");

            // Retrieve the server name from the GreeterAdapter endpoints to identify this server instance.
            String endpoints = communicator.getProperties().getProperty("GreeterAdapter.Endpoints");
            String serverName = "Server(" + endpoints + ")";

            // Register the Chatbot servant with the adapter.
            adapter.add(new Chatbot(serverName), new Identity("greeter", ""));

            // Start dispatching requests.
            adapter.activate();
            System.out.println(serverName + " is listening...");

            // Register a shutdown hook that calls communicator.shutdown() when the user shuts down the server with
            // Ctrl+C or similar. The shutdown hook thread also waits until the main thread completes its cleanup.
            shutdownCommunicatorOnCtrlC(communicator, Thread.currentThread());

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
