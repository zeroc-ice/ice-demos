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
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.SSL.SSLEngineFactory;

final class Server {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create an object adapter.
        try (Communicator communicator = new Communicator(args)) {

            // Create the SSLContext and use it to configure the object adapter. When the adapter accepts a new
            // incoming ssl connection, it uses the `sslEngineFactory` to create an SSLEngine for that connection.
            // Here, we generate these SSLEngine instances using our carefully crafted SSLContext.
            SSLContext sslContext = createSSLContext();
            SSLEngineFactory sslEngineFactory = (String peerHost, int peerPort) -> {
                var engine = sslContext.createSSLEngine(peerHost, peerPort);
                engine.setNeedClientAuth(false);
                return engine;
            };

            // Create an object adapter that listens for incoming requests and dispatches them to servants.
            ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints(
                "GreeterAdapter",
                "ssl -p 4061",
                sslEngineFactory);

            // Register the Chatbot servant with the adapter.
            adapter.add(new Chatbot(), new Identity("greeter", ""));

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

    /**
     * Creates and initializes an SSLContext for use with the SSL transport.
     *
     * <p>The SSLContext is configured with the server's certificate and private key, loaded from a PKCS12 keystore
     * used as the key store to provide the server credentials.
     *
     * @return the initialized SSLContext
     * @throws RuntimeException if an error occurs during SSL initialization
     */
    private static SSLContext createSSLContext() {
        try {
            SSLContext sslContext = SSLContext.getInstance("TLS");
            KeyStore keyStore = KeyStore.getInstance("PKCS12");
            String keyStorePath = "../../../../certs/server.p12";

            // The password for the PKCS12 keystore, hard-coded for simplicity.
            // In a production environment, use a secure method to store and retrieve this password.
            char[] password = "password".toCharArray();

            try (var input = new FileInputStream(keyStorePath)) {
                keyStore.load(input, password);
            }
            KeyManagerFactory keyManagerFactory =
                KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
            keyManagerFactory.init(keyStore, password);
            sslContext.init(keyManagerFactory.getKeyManagers(), null, null);
            return sslContext;
        } catch (
            CertificateException
                | IOException
                | KeyManagementException
                | KeyStoreException
                | NoSuchAlgorithmException
                | UnrecoverableKeyException ex) {
            // Should never happen in this demo.
            throw new RuntimeException("SSL initialization error.", ex);
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
