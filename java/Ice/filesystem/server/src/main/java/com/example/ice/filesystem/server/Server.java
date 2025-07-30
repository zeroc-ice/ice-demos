// Copyright (c) ZeroC, Inc.

package com.example.ice.filesystem.server;

import com.example.filesystem.DirectoryPrx;
import com.example.filesystem.FilePrx;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Util;

class Server {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create an object adapter.
        try (Communicator communicator = Util.initialize(args)) {

            // Register a shutdown hook that calls communicator.shutdown() when the user shuts down the server with
            // Ctrl+C or similar. The shutdown hook thread also waits until the main thread completes its cleanup.
            shutdownCommunicatorOnCtrlC(communicator, Thread.currentThread());

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("Filesystem", "tcp -p 4061");

            // Create the root directory servant (with name "/"), and add this servant to the adapter.
            var root = new MDirectory("/");
            adapter.add(root, Util.stringToIdentity("RootDir"));

            // Create a file called "README", add this servant to the adapter, and add the corresponding proxy to the
            // root directory.
            var file = new MFile("README");
            file.writeDirect(new String[]{"This file system contains a collection of poetry."});
            root.addChild(FilePrx.uncheckedCast(adapter.addWithUUID(file)));

            // Create a directory called "Coleridge", add this servant to the adapter, and add the corresponding proxy
            // to the root directory.
            var coleridge = new MDirectory("Coleridge");
            root.addChild(DirectoryPrx.uncheckedCast(adapter.addWithUUID(coleridge)));

            // Create a file called "Kubla_Khan", add this servant to the adapter, and add the corresponding proxy to
            // the Coleridge directory.
            file = new MFile("Kubla_Khan");
            file.writeDirect(new String[]{
                "In Xanadu did Kubla Khan",
                "A stately pleasure-dome decree:",
                "Where Alph, the sacred river, ran",
                "Through caverns measureless to man",
                "Down to a sunless sea."
            });
            coleridge.addChild(FilePrx.uncheckedCast(adapter.addWithUUID(file)));

            // Start dispatching requests after registering all servants.
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
