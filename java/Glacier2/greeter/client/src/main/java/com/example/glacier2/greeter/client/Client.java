// Copyright (c) ZeroC, Inc.

package com.example.glacier2.greeter.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Util;
import com.zeroc.Glacier2.RouterPrx;
import com.zeroc.Glacier2.SessionPrx;
import com.zeroc.Glacier2.PermissionDeniedException;
import com.zeroc.Glacier2.CannotCreateSessionException;
import java.util.concurrent.ExecutionException;

class Client {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = Util.initialize(args)) {

            // Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is 
            // derived from the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
            RouterPrx router = RouterPrx.createProxy(communicator, "Glacier2/router:tcp -h localhost -p 4063");

            // Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
            // username/password combination. This call establishes a network connection to the Glacier2 router; the 
            // lifetime of the session is the same as the lifetime of the connection.
            SessionPrx session;
            try {
                session = router.createSession(System.getProperty("user.name"), "password");
            } catch (PermissionDeniedException | CannotCreateSessionException e) {
                System.out.println("Could not create session: " + e.getCause());
                return;
            }

            // The proxy returned by createSession is null because we did not configure a SessionManager on the Glacier2
            // router.
            assert session == null;

            // Create a proxy to the Greeter object in the server behind the Glacier2 router. Typically, the client
            // cannot connect directly to this server because it's on an unreachable network.
            GreeterPrx greeter = GreeterPrx.createProxy(communicator, "greeter:tcp -h localhost -p 4061");

            // Configure the proxy to route requests using the Glacier2 router.
            greeter = GreeterPrx.uncheckedCast(greeter.ice_router(router));

            // Send a request to the remote object and get the response with greet.
            String greeting = greeter.greet(System.getProperty("user.name"));
            System.out.println(greeting);
        }
    }
}
