// Copyright (c) ZeroC, Inc.

package com.example.icediscovery.replication.server;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Properties;
import com.zeroc.IceDiscovery.PluginFactory;

import java.util.Collections;
import java.util.UUID;

class Server {
    public static void main(String[] args) {
        var initData = new InitializationData();
        initData.properties = new Properties(args);

        // Configure the communicator to load the IceDiscovery plug-in during initialization. This plug-in installs a
        // default locator on the communicator.
        initData.pluginFactories = Collections.singletonList(new PluginFactory());

        // Generate a unique name for the adapter ID and the greeter name.
        String uuid = UUID.randomUUID().toString();

        // Configure the object adapter GreeterAdapter. It must be an indirect object adapter (i.e., with an AdapterId
        // property); otherwise, the IceDiscovery plug-in can't make it discoverable by IceDiscovery clients.
        // We also set the ReplicaGroupId property to "greeterPool" to enable replication.
        initData.properties.setProperty("GreeterAdapter.AdapterId", String.format("greeter-%s", uuid));
        initData.properties.setProperty("GreeterAdapter.ReplicaGroupId", "greeterPool");

        // Configure the GreeterAdapter to listen on TCP with an OS-assigned port. We don't need a fixed port since the
        // clients discover this object adapter.
        initData.properties.setProperty("GreeterAdapter.Endpoints", "tcp");

        // Create an Ice communicator. We'll use this communicator to create an object adapter.
        try (Communicator communicator = new Communicator(initData)) {

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            // "GreeterAdapter" is a key into the configuration properties set above.
            ObjectAdapter adapter = communicator.createObjectAdapter("GreeterAdapter");

            // Register the Chatbot servant with the adapter.
            adapter.add(new Chatbot(uuid.substring(0, 4)), new Identity("greeter", ""));

            // Start dispatching requests. This method also registers the object adapter with the IceDiscovery plug-in.
            adapter.activate();
            System.out.println("Listening...");

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
