// Copyright (c) ZeroC, Inc.

package com.example.ice.optional.client2;

import com.example.clearsky.AtmosphericConditions;
import com.example.clearsky.WeatherStationPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Util;

import java.security.SecureRandom;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;

class Client {
    private static final SecureRandom random = new SecureRandom();

    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = Util.initialize(args)) {
            // Create a proxy to the weather station.
            var weatherStation = WeatherStationPrx.createProxy(communicator, "weatherStation:tcp -h localhost -p 4061");

            // Create an AtmosphericConditions object with random values.
            double temperature = (random.nextInt(350 - 250) + 250) / 10.0;
            double humidity = (random.nextInt(550 - 450) + 450) / 10.0;
            double pressure = (random.nextInt(10_500 - 10_000) + 10_000) / 10.0;
            AtmosphericConditions reading = new AtmosphericConditions(temperature, humidity, pressure);

            // Report this reading to the weather station.
            weatherStation.report("sensor v1", reading);

            System.out.println("sensor v1: sent reading to weather station");
        }
    }
}
