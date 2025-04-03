#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import sys
from EarlyRiser import AlarmClockPrx, WakeUpServicePrx
from mock_alarm_clock import MockAlarmClock
from common.time import toTimestamp
from datetime import datetime, timedelta

async def main():
    loop = asyncio.get_running_loop()
    # Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and
    # create an object adapter. We enable asyncio support by passing the current event loop to initialize.
    async with Ice.initialize(sys.argv, eventLoop=loop) as communicator:

        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        # Since we don't specify a port, the OS will choose an ephemeral port. This allows multiple client applications
        # to run concurrently on the same host.
        adapter = communicator.createObjectAdapterWithEndpoints("AlarmClockAdapter", "tcp")

        # Register the MockAlarmClock servant with the adapter. The wake up service knows we use identity "alarmClock".
        mockAlarmClock = MockAlarmClock(loop)
        alarmClock = AlarmClockPrx.uncheckedCast(adapter.add(mockAlarmClock, Ice.stringToIdentity("alarmClock")))

        # Start dispatching requests.
        adapter.activate()
        print("Listening on ephemeral port...")

        # Create a proxy to the wake-up service.
        wakeUpService = WakeUpServicePrx(communicator, "wakeUpService:tcp -h localhost -p 4061")

        # Schedule a wake-up call in 5 seconds.
        await wakeUpService.wakeMeUpAsync(alarmClock, toTimestamp(datetime.now() + timedelta(seconds=5)))
        print("Wake-up call scheduled, falling asleep...")

        # Wait until the "stop" button is pressed on the mock alarm clock.
        await mockAlarmClock.waitForStopPressed()
        print("Stop button pressed, exiting...")

if __name__ == "__main__":
    asyncio.run(main())
