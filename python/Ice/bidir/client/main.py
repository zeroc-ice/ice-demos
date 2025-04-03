#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import sys
import EarlyRiser
from mock_alarm_clock import MockAlarmClock
from common.time import toTimestamp
from datetime import datetime, timedelta

async def main():
    loop = asyncio.get_running_loop()
    # Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and
    # create an object adapter. We enable asyncio support by passing the current event loop to initialize.
    async with Ice.initialize(sys.argv, eventLoop=loop) as communicator:

        # Create an object adapter with no name and no configuration. This object adapter does not need to be activated.
        adapter = communicator.createObjectAdapter("")

        # Sets this object adapter as the default object adapter on the communicator.
        communicator.setDefaultObjectAdapter(adapter)

        # Register the MockAlarmClock servant with the adapter. The wake up service knows we use identity "alarmClock".
        mockAlarmClock = MockAlarmClock(loop)
        adapter.add(mockAlarmClock, Ice.stringToIdentity("alarmClock"))

        # Create a proxy to the wake-up service.
        wakeUpService = EarlyRiser.WakeUpServicePrx(communicator, "wakeUpService:tcp -h localhost -p 4061")

        # Schedule a wake-up call in 5 seconds. This call establishes the connection to the server; incoming requests
        # over this connection are handled by the communicator's default object adapter.
        await wakeUpService.wakeMeUpAsync(toTimestamp(datetime.now() + timedelta(seconds=5)))
        print("Wake-up call scheduled, falling asleep...")

        # Wait until the "stop" button is pressed on the mock alarm clock.
        await mockAlarmClock.waitForStopPressed()
        print("Stop button pressed, exiting...")

if __name__ == "__main__":
    asyncio.run(main())
