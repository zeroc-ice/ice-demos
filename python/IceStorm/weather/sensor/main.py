#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import IceStorm
import asyncio
import random
import string
import sys
from datetime import datetime, timezone

# Slice module ClearSky in WeatherStation.ice maps to C# namespace ClearSky.
from ClearSky import AtmosphericConditions, WeatherStationPrx

async def main():
    # Create an Ice communicator. We'll use this communicator to create proxies, and manage outgoing connections. We
    # enable asyncio support by passing the current event loop to initialize.
    async with Ice.initialize(sys.argv, eventLoop=asyncio.get_running_loop()) as communicator:

        # Create a proxy to the IceStorm topic manager.
        topicManager = IceStorm.TopicManagerPrx(communicator, "ClearSky/TopicManager:tcp -p 4061 -h localhost")

        # Retrieve a proxy to the "weather" topic: we first create a topic with the given name (in case we are the
        # first), and then retrieve the proxy if the topic was already created.
        topicName = "weather"
        try:
            topic = await topicManager.createAsync(topicName)
        except IceStorm.TopicExists:
            topic = await topicManager.retrieveAsync(topicName)

        # The proxy returned by createAsync and retrieveAsync is never null.
        assert topic is not None

        # Create a Weather station proxy using the publisher proxy of the topic. The proxy returned by getPublisher is
        # never null.
        publisher = await topic.getPublisherAsync()
        weatherStation = WeatherStationPrx.uncheckedCast(publisher)

        # The proxy returned by IceStorm is a two-way proxy. We can convert it into a oneway proxy if we don't need
        # acknowledgments from IceStorm.
        assert weatherStation.ice_isTwoway()

        # Generate a sensor ID for this weather sensor.
        sensorId = f"sensor-{''.join(random.choices(string.hexdigits, k=4))}"

        # Send a reading every second to the weather station(s) via IceStorm. We keep sending until the user presses Ctrl+C.
        print(f"{sensorId} reporting. Press Ctrl+C to stop...")

        while True:
            # Create a new AtmosphericConditions object with random values.
            reading = AtmosphericConditions(
                temperature = int(random.uniform(190, 230)) / 10.0,
                humidity = int(random.uniform(450, 550)) / 10.0)

            # Send the reading to the weather station(s).
            timeStamp = datetime.now(tz=timezone.utc).strftime("%T")
            await weatherStation.reportAsync(sensorId, timeStamp, reading)

            # Wait for one second before sending the next reading.
            await asyncio.sleep(1.0)

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        # The user pressed Ctrl+C, we exit the while loop.
        pass
