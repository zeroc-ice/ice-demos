# Copyright (c) ZeroC, Inc.

from EarlyRiser import AlarmClock, ButtonPressed
import asyncio
import Ice

class MockAlarmClock(AlarmClock):
    """
    MockAlarmClock is an Ice servant that implements Slice interface AlarmClock
    """

    def __init__(self, eventLoop):
        self._needMoreTime = True
        self._stopPressed = eventLoop.create_future()

    async def waitForStopPressed(self) -> None:
        """
        Asynchronously wait for the ring method to return ButtonPressed.Stop.
        """
        await self._stopPressed

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
            self._needMoreTime = False # we only snooze one time
            return ButtonPressed.Snooze
        else:
            if not self._stopPressed.done():
                # Schedule set_result() to unblock waitForStopPressed() without causing a deadlock. If the future done
                # callback runs synchronously, it could block the event loop waiting for this dispatch to complete. Ice
                # dispatches async methods in the configured event loop, we can use asyncio.get_running_loop to retrieve
                # it.
                asyncio.get_running_loop().call_soon(lambda: self._stopPressed.set_result(None))
            print(f"Returning {ButtonPressed.Stop} to stop the alarm.")
            return ButtonPressed.Stop
