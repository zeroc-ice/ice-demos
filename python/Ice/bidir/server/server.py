#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import time
import Ice
from EarlyRiser import ButtonPressed, WakeUpService, AlarmClockPrx
from common.time import toTimePoint


class BidirWakeUpService(WakeUpService):

    def __init__(self, communicator, eventLoop):
        self._communicator = communicator
        self._eventLoop = eventLoop

    def wakeMeUp(self, timestamp, current):
        timePoint = toTimePoint(timestamp)
        print(f"Dispatching wakeMeUp request {{ timeStamp = '{timePoint.isoformat()}' }}")

        connection = current.con # The connection from the client to the server.
        if (connection is None):
            # Should never happen, but in case it does, the Ice runtime transmits this exception as an
            # Ice.UnknownException.
            raise NotImplementedError("BidirWakeUpService does not support collocated calls")
        
        # Create a proxy to the client's alarm clock. This connection-bound proxy is called a "fixed proxy".
        alarmClock = AlarmClockPrx.uncheckedCast(connection.createProxy(Ice.stringToIdentity("alarmClock")))
        
        self._eventLoop.call_soon_threadsafe(lambda: asyncio.create_task(self.callRing(alarmClock, timePoint)))
    
    async def callRing(self, alarmClock, timePoint):
        delay = timePoint.timestamp() - time.time()
        if delay > 0:
            await asyncio.sleep(delay)

        # First ring. This invocation reuses the connection established by the client.
        buttonPressed = await alarmClock.ringAsync("It's time to wake up!")

        # Keep ringing every 10 seconds until the user presses the stop button.
        while buttonPressed is ButtonPressed.Snooze:
            await asyncio.sleep(10)
            buttonPressed = await alarmClock.ringAsync("No more snoozing!")

