// Copyright (c) ZeroC, Inc.

package com.example.ice.secure.server;

import java.io.FileInputStream;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;

import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Util;

final class Server {
    public static void main(String[] args) {
        // The password for the PKCS12 keystore, hard-coded for simplicity.
        // In a production environment, use a secure method to store and retrieve this password.
        char[] password = "password".toCharArray();

        // Create a KeyStore to load the server's private key and certificate.
        // The PKCS12 format is recommended for modern Java applications.
        KeyStore keyStore;
        try {
            keyStore = KeyStore.getInstance("PKCS12");
        } catch (KeyStoreException ex) {
            throw new RuntimeException("Error initializing PKCS12 keystore.", ex);
        }

        // Load the server's private key and certificate into the KeyStore from the keystore file.
        String keyStorePath = "../../../../certs/server.p12";
        try (var input = new FileInputStream(keyStorePath)) {
            keyStore.load(input, password);
        } catch (Exception ex) {
            throw new RuntimeException("Error loading keystore.", ex);
        }

        // Create a KeyManagerFactory to provide access to the server's private key and certificate.
        KeyManagerFactory keyManagerFactory;
        try {
            keyManagerFactory = KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
            keyManagerFactory.init(keyStore, password);
        } catch (Exception ex) {
            throw new RuntimeException("Error initializing key manager factory.", ex);
        }

        // Create an SSLContext configured for the TLS protocol.
        SSLContext sslContext;
        try {
            sslContext = SSLContext.getInstance("TLS");
        } catch (NoSuchAlgorithmException ex) {
            throw new RuntimeException("Error initializing TLS protocol.", ex);
        }

        // Initialize the SSLContext using the key managers from the KeyManagerFactory.
        try {
            sslContext.init(keyManagerFactory.getKeyManagers(), null, null);
        } catch (Exception ex) {
            throw new RuntimeException("Error initializing SSL context.", ex);
        }

        // Create an Ice communicator. We'll use this communicator to create an object adapter.
        try (Communicator communicator = Util.initialize(args)) {

            // Register a shutdown hook that calls communicator.shutdown() when the user shuts down the server with
            // Ctrl+C or similar. The shutdown hook thread also waits until the main thread completes its cleanup.
            shutdownCommunicatorOnCtrlC(communicator, Thread.currentThread());

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            // The adapter is configured to use the "ssl" secure transport. For each incoming connection,
            // the sslEngineFactory lambda provides an SSLEngine initialized with our custom SSLContext.
            ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints(
                "GreeterAdapter",
                "ssl -p 4061",
                (String peerHost, int peerPort) -> {
                    var engine = sslContext.createSSLEngine(peerHost, peerPort);
                    engine.setNeedClientAuth(false);
                    return engine;
                });

            // Register the Chatbot servant with the adapter.
            adapter.add(new Chatbot(), Util.stringToIdentity("greeter"));

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
