// Copyright (c) ZeroC, Inc.

package com.example.ice.cancellation.server;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Util;
import com.zeroc.Ice.Properties;
import java.util.concurrent.TimeUnit;

class Server {
    public static void main(String[] args) {
        // Set the maximum number of threads in the server thread pool to 3, since Chatbot::greet waits synchronously.
        var initData = new InitializationData();
        initData.properties = new Properties(args);
        initData.properties.setProperty("Ice.ThreadPool.Server.SizeMax", "2");
        // Create an Ice communicator. We'll use this communicator to create an object adapter.
        try (Communicator communicator = Util.initialize(initData)) {

            // Register a shutdown hook that calls communicator.shutdown() when the user shuts down the server with
            // Ctrl+C or similar. The shutdown hook thread also waits until the main thread completes its cleanup.
            shutdownCommunicatorOnCtrlC(communicator, Thread.currentThread());

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061");

            // Register two instances of Chatbot - a regular greater and a slow greeter.
            adapter.add(new Chatbot(0, TimeUnit.SECONDS), Util.stringToIdentity("greeter"));
            adapter.add(new Chatbot(8, TimeUnit.SECONDS), Util.stringToIdentity("slowGreeter"));

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
