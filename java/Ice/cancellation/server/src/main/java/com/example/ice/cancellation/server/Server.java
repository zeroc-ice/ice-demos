// Copyright (c) ZeroC, Inc.

package com.example.ice.cancellation.server;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Properties;

import java.util.concurrent.CompletableFuture;

class Server {
    public static void main(String[] args) {
        // Set the maximum number of threads in the server thread pool to 4, since Chatbot::greet waits synchronously.
        var initData = new InitializationData();
        initData.properties = new Properties(args);
        initData.properties.setProperty("Ice.ThreadPool.Server.SizeMax", "4");

        // Create an Ice communicator. We'll use this communicator to create an object adapter.
        try (Communicator communicator = new Communicator(args)) {
            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

            // Create a CompletableFuture to cancel outstanding dispatches after the user presses Ctrl+C.
            var cancelDispatch = new CompletableFuture<Void>();

            // Register two instances of Chatbot - a regular greeter and a slow greeter.
            adapter.add(new Chatbot(), new Identity("greeter", ""));
            adapter.add(new Chatbot(60, cancelDispatch), new Identity("slowGreeter", ""));

            // Start dispatching requests.
            adapter.activate();
            System.out.println("Listening on port 4061...");

            // Register a shutdown hook that calls communicator.shutdown() when the user shuts down the server with
            // Ctrl+C or similar. The shutdown hook thread also waits until the main thread completes its cleanup.
            shutdownCommunicatorOnCtrlC(communicator, cancelDispatch, Thread.currentThread());

            // Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
            communicator.waitForShutdown();
        }
    }

    private static void shutdownCommunicatorOnCtrlC(
        Communicator communicator, CompletableFuture<Void> cancelDispatch, Thread mainThread) {
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("Caught Ctrl+C, shutting down...");
            communicator.shutdown();

            // Cancel outstanding dispatches "stuck" in the slow greeter.
            cancelDispatch.complete(null);

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
