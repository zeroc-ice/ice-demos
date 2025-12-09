#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import signal
import sys

import Ice
import IceStorm

# Slice module ClearSky in WeatherStation.ice maps to Python module ClearSky.
from ClearSky import WeatherStationPrx
from console_printer import ConsolePrinter


async def main():
    loop = asyncio.get_running_loop()

    # Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and
    # create an object adapter. We enable asyncio support by passing the current event loop to the communicator
    # constructor.
    async with Ice.Communicator(sys.argv, eventLoop=loop) as communicator:
        # Shutdown the communicator when the user presses Ctrl+C.
        signal.signal(signal.SIGINT, lambda signum, frame: communicator.shutdown())

        # Create an object adapter that listens for incoming requests and dispatches them to servants. This object
        # adapter listens on an OS-assigned TCP port, on all interfaces.
        adapter = communicator.createObjectAdapterWithEndpoints("StationAdapter", "tcp")

        # Register the ConsolePrinter servant with the adapter, and get a proxy to the new object.
        # We use a UUID for the identity because these subscribers (weather stations) are transient: if this program
        # exits without unsubscribing its weather station, when it restarts, it logically creates a new weather station
        # as opposed to re-incarnating the old one.
        # See also the Retry Count QoS section in the IceStorm documentation.
        weatherStation = WeatherStationPrx.uncheckedCast(adapter.addWithUUID(ConsolePrinter()))

        # Create a proxy to the IceStorm topic manager.
        topicManager = IceStorm.TopicManagerPrx(communicator, "ClearSky/TopicManager:tcp -p 4061")

        # Ask the topic manager to create or retrieve the "weather" topic and return the corresponding proxy.
        topic = await topicManager.createOrRetrieveAsync("weather")

        # The proxy returned by createOrRetrieveAsync is never null.
        assert topic is not None

        # Start dispatching requests.
        adapter.activate()
        print("Listening...")

        # Register our weather station with the topic.
        # subscribeAndGetPublisherAsync returns a publisher proxy that we don't need here.
        await topic.subscribeAndGetPublisherAsync(theQoS={}, subscriber=weatherStation)

        # Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
        await communicator.shutdownCompleted()

        # Unsubscribe from the topic. The shutdown above only shuts down the object adapter. All client-side
        # functionalities remain available until the communicator is disposed.
        await topic.unsubscribeAsync(weatherStation)
        print(f"Unsubscribed weather station from topic '{topicName}', exiting...")


if __name__ == "__main__":
    asyncio.run(main())
