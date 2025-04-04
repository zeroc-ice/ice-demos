#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import random
import sys
from ClearSky import WeatherStationPrx, AtmosphericConditions

async def main():
    # Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections. We
    # enable asyncio support by passing the current event loop to initialize.
    async with Ice.initialize(sys.argv, eventLoop=asyncio.get_running_loop()) as communicator:

        # Create a proxy to the Weather station.
        weatherStation = WeatherStationPrx(communicator, "weatherStation:tcp -p 4061")

        reading = AtmosphericConditions(
            temperature = int(random.uniform(190, 230)) / 10.0,
            humidity = int(random.uniform(450, 550)) / 10.0)
        
        # Report this reading to the weather station.
        await weatherStation.reportAsync("sensor v1", reading)

        print("sensor v1: sent reading to weather station")

if __name__ == "__main__":
    asyncio.run(main())
