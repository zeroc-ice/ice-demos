#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
from wakeUpService import BidirWakeUpService
import signal
import sys

async def main():

    # Get the current event loop.
    loop=asyncio.get_running_loop()

    # Create an Ice communicator to initialize the Ice runtime. It will be automatically destroyed at the end of the
    # with block. We enable asyncio support by passing the current event loop to initialize.
    with Ice.initialize(sys.argv, eventLoop=loop) as communicator:

        # Shutdown the communicator when the user presses Ctrl+C.
        loop.add_signal_handler(signal.SIGINT, communicator.shutdown)

        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("WakeUpAdapter", "tcp -p 4061")

        # Register the BidirWakeUpService servant with the adapter.
        adapter.add(BidirWakeUpService(communicator, loop), Ice.stringToIdentity("wakeUpService"))

        # Start dispatching requests.
        adapter.activate()
        print("Listening on port 4061...")

        # Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
        await communicator.shutdownCompleted()

if __name__ == "__main__":
    asyncio.run(main())
