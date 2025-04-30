// Copyright (c) ZeroC, Inc.

package com.example.glacier2.greeter.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Util;
import com.zeroc.Glacier2.RouterPrx;
import com.zeroc.Glacier2.SessionPrx;
import com.zeroc.Glacier2.PermissionDeniedException;
import com.zeroc.Glacier2.CannotCreateSessionException;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;

class Client {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = Util.initialize(args)) {

            // Retrieve my username.
            String username = System.getProperty("user.name");

            // Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is derived from
            // the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
            RouterPrx router = RouterPrx.createProxy(communicator, "Glacier2/router:tcp -h localhost -p 4063");

            // If you run the server on a different computer, replace localhost in the string below with the server's
            // hostname or IP address.
            GreeterPrx greeter = GreeterPrx.createProxy(communicator, "greeter:tcp -h localhost -p 4061");

            // Configure the proxy to route requests using the Glacier2 router.
            greeter = GreeterPrx.uncheckedCast(greeter.ice_router(router));

            // Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
            // username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime of the
            // session is the same as the lifetime of the connection.
            SessionPrx session;
            try {
                session = router.createSession(username, "password");
            } catch (PermissionDeniedException | CannotCreateSessionException e) {
                System.out.println("Could not start session: " + e.getCause());
            }

            // Send a request to the remote object and get the response with greetAsync.
            String greeting = greeter.greet(username);
            System.out.println(greeting);
        }
    }
}
