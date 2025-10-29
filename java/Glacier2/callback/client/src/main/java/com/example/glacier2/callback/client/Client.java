// Copyright (c) ZeroC, Inc.

package com.example.glacier2.callback.client;

import java.time.ZoneOffset;
import java.time.ZonedDateTime;
import java.util.concurrent.ExecutionException;

import com.example.earlyriser.AlarmClockPrx;
import com.example.earlyriser.WakeUpServicePrx;
import com.example.util.Time;
import com.zeroc.Glacier2.CannotCreateSessionException;
import com.zeroc.Glacier2.PermissionDeniedException;
import com.zeroc.Glacier2.RouterPrx;
import com.zeroc.Glacier2.SessionPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.ObjectAdapter;

class Client {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = new Communicator(args)) {

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
                System.out.println("Could not create session: " + e.getMessage());
                return;
            }

            // The proxy returned by createSession is null because we did not configure a SessionManager on the Glacier2
            // router.
            assert session == null;

            // Obtain a category string from the router. We need to use this category for the identity of
            // server->client callbacks invoked through the Glacier2 router.
            String clientCategory = router.getCategoryForClient();

            // Create an object adapter with no name and no configuration, but with our router proxy. This object
            // adapter is a "bidirectional" object adapter, like the one created by the Ice/Bidir client application.
            // It does not listen on any port and it does not need to be activated.
            ObjectAdapter adapter = communicator.createObjectAdapterWithRouter("", router);

            // Register the MockAlarmClock servant with the adapter. It uses the category retrieved from the router.
            // You can verify the Ring callback is never delivered if you provide a different category.
            var mockAlarmClock = new MockAlarmClock();
            AlarmClockPrx alarmClock = AlarmClockPrx.uncheckedCast(
                adapter.add(mockAlarmClock, new Identity("alarmClock", clientCategory)));

            // Create a proxy to the wake-up service behind the Glacier2 router. Typically, the client cannot connect
            // directly to this server because it's on an unreachable network.
            WakeUpServicePrx wakeUpService = WakeUpServicePrx.createProxy(
                communicator,
                "wakeUpService:tcp -h localhost -p 4061");

            // Configure the proxy to route requests using the Glacier2 router.
            wakeUpService = WakeUpServicePrx.uncheckedCast(wakeUpService.ice_router(router));

            // Schedule a wake-up call in 5 seconds. This call establishes the connection to the server; incoming
            // requests over this connection are handled by the communicator's default object adapter.
            wakeUpService.wakeMeUp(alarmClock, Time.toTimeStamp(ZonedDateTime.now(ZoneOffset.UTC).plusSeconds(5)));
            System.out.println("Wake-up call scheduled, falling asleep...");
            try {
                // Wait until the user presses the stop button on the alarm clock.
                mockAlarmClock.stopPressed().get();
                System.out.println("Stop button pressed, exiting...");
            } catch (InterruptedException | ExecutionException ex) {
                System.err.println(ex);
            }
        }
    }
}
