# Copyright (c) ZeroC, Inc.

import asyncio

import Ice
from EarlyRiser import AlarmClock, ButtonPressed


class MockAlarmClock(AlarmClock):
    """
    MockAlarmClock is an Ice servant that implements Slice interface AlarmClock.
    """

    def __init__(self, stopPressed: asyncio.Future):
        """
        Initializes the MockAlarmClock object.

        Parameters
        ----------
        stopPressed : asyncio.Future
            A future that is set when the Stop button is pressed on the alarm clock.
        """
        self._needMoreTime = True
        self._stopPressed = stopPressed

    async def ring(self, message: str, current: Ice.Current) -> ButtonPressed:
        """
        Implements the abstract method ring from the AlarmClock class generated by the Slice compiler.

        Parameters
        ----------
        message : str
            The message to display on the alarm clock.
        current : Ice.Current
            The Current object for the dispatch.
        """

        print(f"Dispatching ring request {{ message = '{message}' }}")
        if self._needMoreTime:
            print(f"Returning {ButtonPressed.Snooze} to request more time.")
            self._needMoreTime = False  # we only snooze one time
            return ButtonPressed.Snooze
        else:
            if not self._stopPressed.done():
                self._stopPressed.set_result(None)
            print(f"Returning {ButtonPressed.Stop} to stop the alarm.")
            return ButtonPressed.Stop
