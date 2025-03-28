# Copyright (c) ZeroC, Inc.

from EarlyRiser import AlarmClock, ButtonPressed

class MockAlarmClock(AlarmClock):

    def __init__(self, loop):
        self._needMoreTime = True
        self._loop = loop
        self._stopPressed = loop.create_future()

    async def waitForStopPressed(self):
        await self._stopPressed
    
    def stop(self):
        if not self._stopPressed.done():
            self._stopPressed.set_result(None)

    def ring(self, message, current):
        print(f"Dispatching ring request {{ message = '{message}' }}")
        if self._needMoreTime:
            print(f"Returning {ButtonPressed.Snooze} to request more time.")
            self._needMoreTime = False # we only snooze one time
            return ButtonPressed.Snooze
        else:
            self._loop.call_soon_threadsafe(self.stop)
            print(f"Returning {ButtonPressed.Stop} to stop the alarm.")
            return ButtonPressed.Stop
