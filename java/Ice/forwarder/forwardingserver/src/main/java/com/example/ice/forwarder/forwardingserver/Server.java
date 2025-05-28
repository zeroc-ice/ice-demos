// Copyright (c) ZeroC, Inc.

package com.example.ice.forwarder.forwardingserver;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.ObjectPrx;
import com.zeroc.Ice.Util;

class Server {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create an object adapter, and to create proxies
        // and manage outgoing connections.
        try (Communicator communicator = Util.initialize(args)) {

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("ForwarderAdapter", "tcp -p 10000");

            // Create a target proxy template, with a dummy identity.
            ObjectPrx targetTemplate = ObjectPrx.createProxy(communicator, "dummy:tcp -h localhost -p 4061");

            // Register the Forwarder servant as a default servant with the object adapter. The empty category means
            // this default servant receives requests to all Ice objects.
            adapter.addDefaultServant(new Forwarder(targetTemplate), "");

            // Start dispatching requests.
            adapter.activate();
            System.out.println("Listening on port 10000...");

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
