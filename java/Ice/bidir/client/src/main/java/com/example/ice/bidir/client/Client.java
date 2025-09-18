// Copyright (c) ZeroC, Inc.

package com.example.ice.bidir.client;

import java.time.ZoneOffset;
import java.time.ZonedDateTime;
import java.util.concurrent.ExecutionException;

import com.example.earlyriser.WakeUpServicePrx;
import com.example.util.Time;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Util;

class Client {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and
        // create an object adapter.
        try (Communicator communicator = Util.initialize(args)) {
            // Create an object adapter with no name and no configuration. This object adapter does not need to be
            // activated.
            ObjectAdapter adapter = communicator.createObjectAdapter("");

            // Sets this object adapter as the default object adapter on the communicator.
            communicator.setDefaultObjectAdapter(adapter);

            // Register the MockAlarmClock servant with the adapter. The wake up service knows we use identity
            // "alarmClock".
            var mockAlarmClock = new MockAlarmClock();
            adapter.add(mockAlarmClock, new Identity("alarmClock", ""));

            // Create a proxy to the wake-up service.
            WakeUpServicePrx wakeUpService = WakeUpServicePrx.createProxy(
                communicator,
                "wakeUpService:tcp -h localhost -p 4061");

            // Schedule a wake-up call in 5 seconds. This call establishes the connection to the server; incoming
            // requests over this connection are handled by the communicator's default object adapter.
            wakeUpService.wakeMeUp(Time.toTimeStamp(ZonedDateTime.now(ZoneOffset.UTC).plusSeconds(5)));
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
