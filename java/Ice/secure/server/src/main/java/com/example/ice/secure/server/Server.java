// Copyright (c) ZeroC, Inc.

package com.example.ice.secure.server;

import java.io.FileInputStream;
import java.io.IOException;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.UnrecoverableKeyException;
import java.security.cert.CertificateException;

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

        // Create and initialize an SSLContext object for the TLS protocol. The SSLContext is configured with the
        // server's private key and certificate, which are loaded from a PKCS12 keystore. This SSLContext will be used
        // to configure the server's object adapter.
        SSLContext sslContext;
        try {
            sslContext = SSLContext.getInstance("TLS");
            KeyStore keyStore = KeyStore.getInstance("PKCS12");
            String keyStorePath = "../../../../certs/server.p12";
            try (var input = new FileInputStream(keyStorePath)) {
                keyStore.load(input, password);
            }
            KeyManagerFactory keyManagerFactory =
                KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
            keyManagerFactory.init(keyStore, password);
            sslContext.init(keyManagerFactory.getKeyManagers(), null, null);
        }  catch (
            CertificateException|
            IOException|
            KeyManagementException|
            KeyStoreException|
            NoSuchAlgorithmException|
            UnrecoverableKeyException ex) {
            throw new RuntimeException("SSL initialization error.", ex);
        }

        // Create an Ice communicator. We'll use this communicator to create an object adapter.
        try (Communicator communicator = Util.initialize(args)) {

            // Register a shutdown hook that calls communicator.shutdown() when the user shuts down the server with
            // Ctrl+C or similar. The shutdown hook thread also waits until the main thread completes its cleanup.
            shutdownCommunicatorOnCtrlC(communicator, Thread.currentThread());

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            // We configure the object adapter to use the "ssl" transport and our custom SSLContext.
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
