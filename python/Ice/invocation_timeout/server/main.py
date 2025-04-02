#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import chatbot
import signal
import sys

async def main():
    # Get the current event loop.
    loop=asyncio.get_running_loop()

    # Create an Ice communicator. We'll use this communicator to create an object adapter. We enable asyncio
    # support by passing the current event loop to initialize.
    with Ice.initialize(sys.argv, eventLoop=loop) as communicator:
        # Shutdown the communicator when the user presses Ctrl+C.
        loop.add_signal_handler(signal.SIGINT, communicator.shutdown)

        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061")

        # Register two instances of Chatbot - a regular greater and a slow greeter.
        adapter.add(chatbot.Chatbot(0.0), Ice.stringToIdentity("greeter"))
        adapter.add(chatbot.Chatbot(60.0), Ice.stringToIdentity("slowGreeter"))

        # Start dispatching requests.
        adapter.activate()
        print("Listening on port 4061...")

        # Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
        await communicator.shutdownCompleted()

if __name__ == "__main__":
    asyncio.run(main())
