// Copyright (c) ZeroC, Inc.

package com.example.glacier2.session.server;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.ObjectAdapter;

class Server {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create an object adapter.
        try (Communicator communicator = new Communicator(args)) {

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("PokeAdapter", "tcp -p 4061");

            // Register the SessionManager servant with the adapter.
            var sessionManager = new SessionManager(adapter);
            adapter.add(sessionManager, new Identity("SessionManager", ""));

            // Register the SharedPokeBox servant with the adapter as a default servant for category "PokeBox".
            // This servant implements all PokeBox objects.
            var sharedPokeBox = new SharedPokeBox(new InMemoryPokeStore(), sessionManager);
            adapter.addDefaultServant(sharedPokeBox, "PokeBox");

            // Start dispatching requests.
            adapter.activate();
            System.out.println("Listening on port 4061...");

            // Register a shutdown hook that calls `communicator.shutdown()` when the user presses 'Ctrl+C' or similar.
            // The shutdown hook thread waits until the main thread completes its cleanup.
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
            } catch (InterruptedException ignored) {}
        }));
    }
}
