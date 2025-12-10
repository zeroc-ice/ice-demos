// Copyright (c) ZeroC, Inc.

package com.example.icestorm.weather.sensor;

import java.security.SecureRandom;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.time.format.FormatStyle;
import java.util.Locale;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import com.example.clearsky.AtmosphericConditions;
import com.example.clearsky.WeatherStationPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.ObjectPrx;
import com.zeroc.IceStorm.TopicManagerPrx;
import com.zeroc.IceStorm.TopicPrx;

class Sensor {
    private static final SecureRandom random = new SecureRandom();

    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = new Communicator(args)) {
            // Create a proxy to the IceStorm topic manager.
            TopicManagerPrx topicManager = TopicManagerPrx.createProxy(
                communicator, "ClearSky/TopicManager:tcp -p 4061 -h localhost");

            // Ask the topic manager to create or retrieve the "weather" topic and return the corresponding proxy.
            TopicPrx topic = topicManager.createOrRetrieve("weather");

            // The proxy returned by createOrRetrieve is never null.
            assert topic != null;

            ObjectPrx publisher = topic.getPublisher();
            // The proxy returned by getPublisher is never null.
            assert publisher != null;

            // Create a WeatherStation proxy using the publisher proxy of the topic.
            WeatherStationPrx weatherStation = WeatherStationPrx.uncheckedCast(publisher);

            // The proxy returned by IceStorm is a two-way proxy. We can convert it into a oneway proxy if we don't
            // need acknowledgments from IceStorm.
            assert weatherStation.ice_isTwoway();

            // Generate a sensor ID for this weather sensor.
            String sensorId = String.format("sensor-%08x", random.nextInt());
            System.out.println(sensorId + " reporting. Press Ctrl+C to stop...");

            DateTimeFormatter formatter = DateTimeFormatter
                .ofLocalizedTime(FormatStyle.MEDIUM)
                .withLocale(Locale.getDefault());

            var stopFuture = new CompletableFuture<Void>();
            // Complete the stop future when the user presses Ctrl+C to terminate the program.
            completeFutureOnCtrlC(stopFuture, Thread.currentThread());

            // Send a reading every second to the weather station(s) via IceStorm. We keep sending until the user
            // presses Ctrl+C.
            while (true) {
                double temperature = random.nextInt(250, 350) / 10.0;
                double humidity = random.nextInt(450, 550) / 10.0;
                AtmosphericConditions reading = new AtmosphericConditions(temperature, humidity);

                String timeStamp = LocalTime.now().format(formatter);

                weatherStation.report(sensorId, timeStamp, reading);

                try {
                    // Wait for one second or until the shutdown future is completed when the user presses Ctrl+C.
                    stopFuture.get(1, TimeUnit.SECONDS);
                    break; // shutdown future was set
                } catch (TimeoutException ex) {
                    // The wait timed out, so send the next reading.
                } catch (InterruptedException | ExecutionException ex) {
                    // This should not happen because:
                    // - The thread is never interrupted in this demo
                    // - The shutdown future is never completed exceptionally
                    assert false;
                }
            }
        }
    }

    private static void completeFutureOnCtrlC(CompletableFuture<Void> stopFuture, Thread mainThread) {
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("Caught Ctrl+C, stopping...");
            stopFuture.complete(null);
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
