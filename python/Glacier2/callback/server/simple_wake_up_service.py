#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import time
import Ice
from EarlyRiser import ButtonPressed, WakeUpService, AlarmClockPrx
from datetime import datetime
from common.time import toDatetime

async def callRing(alarmClock: AlarmClockPrx, wakeUpDatetime: datetime):
    """
    Asynchronously call the ring method on the alarm clock proxy until the user presses the stop button.
    The first ring is delayed until the specified wakeUpDatetime. Then the method will keep ringing every 10
    seconds until the user presses the stop button.

    Parameters
    ----------
    alarmClock : AlarmClockPrx
        The proxy to the client's alarm clock.
    wakeUpDatetime : datetime
        The datetime when the alarm should ring.
    """
    delay = wakeUpDatetime.timestamp() - time.time()
    if delay > 0:
        await asyncio.sleep(delay)

    # First ring. This invocation reuses the connection established by the client.
    buttonPressed = await alarmClock.ringAsync("It's time to wake up!")

    # Keep ringing every 10 seconds until the user presses the stop button.
    while buttonPressed is ButtonPressed.Snooze:
        await asyncio.sleep(10)
        buttonPressed = await alarmClock.ringAsync("No more snoozing!")


class SimpleWakeUpService(WakeUpService):
    """
    SimpleWakeUpService is an Ice servant that implements Slice interface WakeUpService.
    """

    async def wakeMeUp(self, alarmClock: AlarmClockPrx, timestamp: int, current: Ice.Current) -> None:
        """
        Implements the abstract method wakeMeUp from the WakeUpService class generated by the Slice compiler.

        Parameters
        ----------
        alarmClock : AlarmClockPrx
            The proxy to the client's alarm clock.
        timestamp : int
            The timestamp represented as the number of ticks since January 1, 0001 00:00:00 UTC.
            Each tick is 100 nanoseconds.
        current : Ice.Current
            The Current object for the dispatch.
        """
        wakeUpDatetime = toDatetime(timestamp)
        print(f"Dispatching wakeMeUp request {{ timeStamp = '{wakeUpDatetime.astimezone()}' }}")

        # Schedule the callRing coroutine to run in the asyncio event loop. Since Ice dispatches async methods in the
        # configured event loop, we can use asyncio.get_running_loop to retrieve it.
        asyncio.get_running_loop().create_task(callRing(alarmClock, wakeUpDatetime))
        print("Client pressed Stop on alarm clock.")
