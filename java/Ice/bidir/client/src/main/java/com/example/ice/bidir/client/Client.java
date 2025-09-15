// Copyright (c) ZeroC, Inc.

package com.example.ice.bidir.client;

import com.example.earlyriser.WakeUpServicePrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Util;

import java.io.Console;
import java.time.ZoneOffset;
import java.time.ZonedDateTime;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;

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
            long millis = ZonedDateTime.now(ZoneOffset.UTC)
                           .plusSeconds(10)
                           .toInstant()
                           .toEpochMilli();

            long millisecondsBeforeEpoch = 719162 * 24 * 60 * 60 * 1000; // daysBeforeEpoch converted to milliseconds
            long ticksPerMillisecond = 10_000; // number of ticks per millisecond
            long ticks = millis * ticksPerMillisecond + millisecondsBeforeEpoch;

            wakeUpService.wakeMeUp(ticks);
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
