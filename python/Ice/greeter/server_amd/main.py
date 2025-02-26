#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import chatbot
import sys

async def main():
    # Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
    with Ice.initialize(sys.argv) as communicator:
        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061")

        # Register the Chatbot servant with the adapter.
        adapter.add(chatbot.Chatbot(asyncio.get_running_loop()), Ice.Util.stringToIdentity("greeter"))

        # Start dispatching requests.
        adapter.activate()
        print("Listening on port 4061...")

        # Create an asyncio event to wait forever.
        try:
            await asyncio.Event().wait()
        except asyncio.exceptions.CancelledError:
            print("Caught Ctrl+C, exiting")

# Craete the asyncio event loop and run the main couroutine on it.
asyncio.run(main())
