// Copyright (c) ZeroC, Inc.

package com.example.ice.optional.server1;

import com.example.clearsky.AtmosphericConditions;
import com.zeroc.Ice.ClassSliceLoader;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Properties;
import com.zeroc.Ice.Util;

class Server {
    public static void main(String[] args) {
        // We need to configure a Slice loader because we receive a class that was mapped to a non-default package with
        // java:identifier.
        var initData = new InitializationData();
        initData.properties = new Properties(args);
        initData.sliceLoader = new ClassSliceLoader(AtmosphericConditions.class);

        // Create a communicator with initData. We'll use this communicator to create an object adapter.
        try (Communicator communicator = Util.initialize(initData)) {

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("StationAdapter", "tcp -p 4061");

            // Register the ConsolePrinter servant with the adapter.
            adapter.add(new ConsolePrinter(), new Identity("weatherStation", ""));

            // Start dispatching requests.
            adapter.activate();
            System.out.println("Listening on port 4061...");

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
