#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import signal
import sys

import chatbot
import Ice


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
        adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061")

        # Register two instances of Chatbot - a regular greater and a slow greeter.
        adapter.add(chatbot.Chatbot(0.0), Ice.Identity(name="greeter"))
        adapter.add(chatbot.Chatbot(60.0), Ice.Identity(name="slowGreeter"))

        # Start dispatching requests.
        adapter.activate()
        print("Listening on port 4061...")

        # Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
        await communicator.shutdownCompleted()


if __name__ == "__main__":
    asyncio.run(main())
