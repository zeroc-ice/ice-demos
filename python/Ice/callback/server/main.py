#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import sys

import Ice
from simple_wake_up_service import SimpleWakeUpService


async def main():
    loop = asyncio.get_running_loop()

    # Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and
    # create an object adapter. We enable asyncio support by passing the current event loop to the communicator
    # constructor.
    async with Ice.Communicator(sys.argv, eventLoop=loop) as communicator:
        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("WakeUpAdapter", "tcp -p 4061")

        # Register the SimpleWakeUpService servant with the adapter.
        adapter.add(SimpleWakeUpService(), Ice.Identity(name="wakeUpService"))

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
