#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import chatbot
import signal
import sys

async def main():
    # Create an event to wait for Ctrl+C signal
    exit_event = asyncio.Event()

    loop = asyncio.get_running_loop()
    loop.add_signal_handler(signal.SIGINT, lambda: exit_event.set())

    # Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
    with Ice.initialize(sys.argv) as communicator:
        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061")

        adapter.add(chatbot.Chatbot(loop), Ice.Util.stringToIdentity("greeter"))

        # Start dispatching requests.
        adapter.activate()
        print("Listening on port 4061...")

        # Wait until exit_event is set (e.g., via Ctrl+C)
        await exit_event.wait()
        print("Caught Ctrl+C, exiting...")

asyncio.run(main())
