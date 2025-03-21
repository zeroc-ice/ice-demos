// Copyright (c) ZeroC, Inc.

package com.zeroc.demos.greeter;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.ObjectAdapter;

class Server {
    public static void main(String[] args) {
        // Create an Ice communicator to initialize the Ice runtime. The communicator is closed (destroyed) at the end
        // of the try statement.
        try (Communicator communicator = com.zeroc.Ice.Util.initialize(args)) {

            // Register a shutdown hook that calls communicator.shutdown() when the user shuts down the server with
            // Ctrl+C or similar. The shutdown hook thread also waits until the main thread completes its cleanup.
            shutdownCommunicatorOnCtrlC(communicator, Thread.currentThread());

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

            // Register the Chatbot servant with the adapter.
            adapter.add(new Chatbot(), com.zeroc.Ice.Util.stringToIdentity("greeter"));

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
                assert false : "Shutdown thread cannot be interrupted";
            }
        }));
    }
}
