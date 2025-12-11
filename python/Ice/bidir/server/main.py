#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import sys

import Ice
from bidir_wake_up_service import BidirWakeUpService


async def main():
    # Get the current event loop.
    loop = asyncio.get_running_loop()

    # Create an Ice communicator. We'll use this communicator to create an object adapter. We enable asyncio
    # support by passing the current event loop to the communicator constructor.
    async with Ice.Communicator(sys.argv, eventLoop=loop) as communicator:
        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("WakeUpAdapter", "tcp -p 4061")

        # Register the BidirWakeUpService servant with the adapter.
        adapter.add(BidirWakeUpService(), Ice.Identity(name="wakeUpService"))

        # Start dispatching requests.
        adapter.activate()
        print("Listening on port 4061...")

        # Wait until the communicator is shut down (never happens here) or the user presses Ctrl+C.
        try:
            await communicator.shutdownCompleted()
        except asyncio.CancelledError:
            print("Caught Ctrl+C, shutting down...")


if __name__ == "__main__":
    asyncio.run(main())
