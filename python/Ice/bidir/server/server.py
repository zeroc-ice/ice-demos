#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import time
import Ice
from EarlyRiser import ButtonPressed, WakeUpService, AlarmClockPrx
from common.time import toDatetime


class BidirWakeUpService(WakeUpService):

    def __init__(self, communicator, eventLoop):
        self._communicator = communicator
        self._eventLoop = eventLoop

    def wakeMeUp(self, timestamp, current):
        datetime = toDatetime(timestamp)
        print(f"Dispatching wakeMeUp request {{ timeStamp = '{datetime.astimezone()}' }}")

        connection = current.con # The connection from the client to the server.
        if (connection is None):
            # Should never happen, but in case it does, the Ice runtime transmits this exception as an
            # Ice.UnknownException.
            raise NotImplementedError("BidirWakeUpService does not support collocated calls")
        
        # Create a proxy to the client's alarm clock. This connection-bound proxy is called a "fixed proxy".
        alarmClock = AlarmClockPrx.uncheckedCast(connection.createProxy(Ice.stringToIdentity("alarmClock")))
        
        asyncio.run_coroutine_threadsafe(self.callRing(alarmClock, datetime), self._eventLoop)
    
    async def callRing(self, alarmClock, datetime):
        delay = datetime.timestamp() - time.time()
        if delay > 0:
            await asyncio.sleep(delay)

        # First ring. This invocation reuses the connection established by the client.
        buttonPressed = await alarmClock.ringAsync("It's time to wake up!")

        # Keep ringing every 10 seconds until the user presses the stop button.
        while buttonPressed is ButtonPressed.Snooze:
            await asyncio.sleep(10)
            buttonPressed = await alarmClock.ringAsync("No more snoozing!")

