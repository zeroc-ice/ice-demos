#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import signal
import sys

import Ice
from bidir_wake_up_service import BidirWakeUpService


async def main():
    # Get the current event loop.
    loop = asyncio.get_running_loop()

    # Create an Ice communicator. We'll use this communicator to create an object adapter. We enable asyncio
    # support by passing the current event loop to initialize.
    async with Ice.initialize(sys.argv, eventLoop=loop) as communicator:
        # Shutdown the communicator when the user presses Ctrl+C.
        loop.add_signal_handler(signal.SIGINT, communicator.shutdown)

        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("WakeUpAdapter", "tcp -p 4061")

        # Register the BidirWakeUpService servant with the adapter.
        adapter.add(BidirWakeUpService(), Ice.Identity(name="wakeUpService"))

        # Start dispatching requests.
        adapter.activate()
        print("Listening on port 4061...")

        # Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
        await communicator.shutdownCompleted()


if __name__ == "__main__":
    asyncio.run(main())
