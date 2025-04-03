#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import Glacier2
import asyncio
import getpass
import sys
from EarlyRiser import AlarmClockPrx, WakeUpServicePrx
from mock_alarm_clock import MockAlarmClock
from common.time import toTimestamp
from datetime import datetime, timedelta

async def main():
    loop = asyncio.get_running_loop()
    # Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and
    # create an object adapter. We enable asyncio support by passing the current event loop to initialize.
    with Ice.initialize(sys.argv, eventLoop=loop) as communicator:

        # Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is
        # derived from the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
        router = Glacier2.RouterPrx(communicator, "Glacier2/router:tcp -h localhost -p 4063")

        # Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
        # username/password combination. This call establishes a network connection to the Glacier2 router; the
        # lifetime of the session is the same as the lifetime of the connection.
        session = await router.createSessionAsync(getpass.getuser(), "password")

        # The proxy returned by createSession is None because we did not configure a SessionManager on the Glacier2
        # router.
        assert session is None

        # Obtain a category string from the router. We need to use this category for the identity of server->client
        # callbacks invoked through the Glacier2 router.
        clientCategory = await router.getCategoryForClientAsync();

        # Create an object adapter with no name and no configuration, but with our router proxy. This object adapter is
        # a "bidirectional" object adapter, like the one created by the Ice/bidir client application. It does not
        # listen on any port and it does not need to be activated.
        adapter = communicator.createObjectAdapterWithRouter("", router)

        # Register the MockAlarmClock servant with the adapter. It uses the category retrieved from the router. You can
        # verify the Ring callback is never delivered if you provide a different category.
        mockAlarmClock = MockAlarmClock(loop)
        alarmClock = AlarmClockPrx.uncheckedCast(
            adapter.add(mockAlarmClock, Ice.Identity(name="alarmClock", category=clientCategory)))

        # Create a proxy to the wake-up service.
        wakeUpService = WakeUpServicePrx(communicator, "wakeUpService:tcp -h localhost -p 4061")

        # Configure the proxy to route requests using the Glacier2 router.
        wakeUpService = wakeUpService.ice_router(router)

        # Schedule a wake-up call in 5 seconds.
        await wakeUpService.wakeMeUpAsync(alarmClock, toTimestamp(datetime.now() + timedelta(seconds=5)))
        print("Wake-up call scheduled, falling asleep...")

        # Wait until the "stop" button is pressed on the mock alarm clock.
        await mockAlarmClock.waitForStopPressed()
        print("Stop button pressed, exiting...")

if __name__ == "__main__":
    asyncio.run(main())
