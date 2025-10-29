#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import random
import sys

import Ice
from ClearSky import AtmosphericConditions, WeatherStationPrx


async def main():
    # Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections. We
    # enable asyncio support by passing the current event loop to the communicator constructor.
    async with Ice.Communicator(sys.argv, eventLoop=asyncio.get_running_loop()) as communicator:
        # Create a proxy to the Weather station.
        weatherStation = WeatherStationPrx(communicator, "weatherStation:tcp -p 4061")

        reading = AtmosphericConditions(
            temperature=int(random.uniform(190, 230)) / 10.0,
            humidity=int(random.uniform(450, 550)) / 10.0,
            pressure=int(random.uniform(10_000, 10_500)) / 10.0,
        )

        # Report this reading to the weather station.
        await weatherStation.reportAsync("sensor v2", reading)

        print("sensor v2: sent reading to weather station")


if __name__ == "__main__":
    asyncio.run(main())
