#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import signal
import sys

import Ice
from poke_session import InMemoryPokeStore, SessionManager, SharedPokeBox


async def main():
    # Get the current event loop.
    loop = asyncio.get_running_loop()

    # Create an Ice communicator. We'll use this communicator to create an object adapter. We enable asyncio
    # support by passing the current event loop to the communicator constructor.
    async with Ice.Communicator(sys.argv, eventLoop=loop) as communicator:
        # Shutdown the communicator when the user presses Ctrl+C.
        try:
            loop.add_signal_handler(signal.SIGINT, communicator.shutdown)
        except NotImplementedError:
            # asyncio signal handlers are not implemented on Windows.
            signal.signal(signal.SIGINT, lambda signum, frame: communicator.shutdown())

        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("PokeAdapter", "tcp -p 4061")

        # Register the SessionManager servant with the adapter.
        sessionManager = SessionManager(adapter)
        adapter.add(sessionManager, Ice.Identity(name="SessionManager"))

        # Register the SharedPokeBox servant with the adapter as a default servant for category "PokeBox". This servant
        # implements all PokeBox objects.
        sharedPokeBox = SharedPokeBox(InMemoryPokeStore(), sessionManager)
        adapter.addDefaultServant(sharedPokeBox, "PokeBox")

        # Start dispatching requests.
        adapter.activate()
        print("Listening on port 4061...")

        # Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
        await communicator.shutdownCompleted()


if __name__ == "__main__":
    asyncio.run(main())
