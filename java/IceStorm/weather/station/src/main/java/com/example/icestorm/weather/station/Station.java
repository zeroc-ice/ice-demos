// Copyright (c) ZeroC, Inc.

package com.example.icestorm.weather.station;

import com.example.clearsky.AtmosphericConditions;
import com.example.clearsky.WeatherStationPrx;
import com.zeroc.Ice.ClassSliceLoader;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Properties;
import com.zeroc.IceStorm.AlreadySubscribed;
import com.zeroc.IceStorm.BadQoS;
import com.zeroc.IceStorm.NoSuchTopic;
import com.zeroc.IceStorm.TopicExists;
import com.zeroc.IceStorm.TopicManagerPrx;
import com.zeroc.IceStorm.TopicPrx;

import java.util.Map;

class Station {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and
        // create an object adapter.
        var initData = new InitializationData();
        initData.properties = new Properties(args);
        initData.sliceLoader = new ClassSliceLoader(AtmosphericConditions.class);
        try (Communicator communicator = new Communicator(initData)) {

            // Create an object adapter that listens for incoming requests and dispatches them to servants. This object
            // adapter listens on an OS-assigned TCP port, on all interfaces.
            ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("StationAdapter", "tcp");

            // Register the ConsolePrinter servant with the adapter, and get a proxy to the new object.
            // We use a UUID for the identity because these subscribers (weather stations) are transient: if this
            // program exits without unsubscribing its weather station, when it restarts, it logically creates a new
            // weather station as opposed to re-incarnating the old one.
            // See also the Retry Count QoS section in the IceStorm documentation.
            WeatherStationPrx weatherStation = WeatherStationPrx.uncheckedCast(
                adapter.addWithUUID(new ConsolePrinter()));

            // Create a proxy to the IceStorm topic manager.
            TopicManagerPrx topicManager = TopicManagerPrx.createProxy(
                communicator, "ClearSky/TopicManager:tcp -p 4061");

            // Retrieve a proxy to the "weather" topic: we first create a topic with the given name (in case we are the
            // first), and then retrieve the proxy if the topic was already created.
            TopicPrx topic;
            String topicName = "weather";
            try {
                topic = topicManager.create(topicName);
            } catch (TopicExists topicExists) {
                try {
                    topic = topicManager.retrieve(topicName);
                } catch (NoSuchTopic noSuchTopic) {
                    assert false;
                    return;
                }
            }

            // The proxy returned by create and retrieve is never null.
            assert topic != null;

            // Start dispatching requests.
            adapter.activate();
            System.out.println("Listening...");

            // Register our weather station with the topic.
            // subscribeAndGetPublisher returns a publisher proxy that we don't need here.
            Map<String, String> qos = Map.of();
            try {
                topic.subscribeAndGetPublisher(qos, weatherStation);
            } catch (BadQoS | AlreadySubscribed ex) {
                assert false;
            }
            System.out.println("Subscribed weather station to topic '" + topicName + "'.");

            // Register a shutdown hook that calls communicator.shutdown() when the user shuts down the server with
            // Ctrl+C or similar. The shutdown hook thread also waits until the main thread completes its cleanup.
            shutdownCommunicatorOnCtrlC(communicator, Thread.currentThread());

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
