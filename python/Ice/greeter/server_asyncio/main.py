#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import chatbot
import signal

async def main():

    # Get the current event loop.
    loop=asyncio.get_running_loop()

    # Create an Ice communicator to initialize the Ice runtime. It will be automatically destroyed at the end of the
    # with block.
    #
    # The event loop is passed to Ice.initialize so that the communicator is configured to use the
    # Ice.asyncio.EventLoopAdapter, ensuring that:
    # - Futures returned by the Ice runtime can be awaited within the asyncio event loop.
    # - Coroutines from asynchronous dispatches run within the same event loop.
    with Ice.initialize(eventLoop=loop) as communicator:

        # Shutdown the communicator when the user presses Ctrl+C.
        loop.add_signal_handler(signal.SIGINT, communicator.shutdown)

        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061")

        # Register the Chatbot servant with the adapter.
        adapter.add(chatbot.Chatbot(), Ice.Util.stringToIdentity("greeter"))

        # Start dispatching requests.
        adapter.activate()
        print("Listening on port 4061...")

        # Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
        await communicator.shutdownCompleted()

asyncio.run(main())
