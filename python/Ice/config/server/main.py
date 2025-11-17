#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import signal
import sys

import chatbot
import Ice


async def main():
    # Load the contents of the config.server file into a Properties object.
    configFileProperties = Ice.Properties()
    configFileProperties.load("config.server")

    initData = Ice.InitializationData()

    # Create a Properties object from the command line arguments and the config file properties; Ice.* properties and
    # other reserved properties set in the sys.argv command-line arguments override the config file properties.
    initData.properties = Ice.Properties(sys.argv, configFileProperties)

    # Configure the communicator to use asyncio.
    loop = asyncio.get_running_loop()
    initData.eventLoopAdapter = Ice.asyncio.EventLoopAdapter(loop)

    # Create an Ice communicator. We'll use this communicator to create an object adapter.
    # The communicator gets its properties from the properties object.
    async with Ice.Communicator(initData=initData) as communicator:
        # Shutdown the communicator when the user presses Ctrl+C.
        try:
            loop.add_signal_handler(signal.SIGINT, communicator.shutdown)
        except NotImplementedError:
            # asyncio signal handlers are not implemented on Windows.
            signal.signal(signal.SIGINT, lambda signum, frame: communicator.shutdown())

        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapter("GreeterAdapter")

        # Register the Chatbot servant with the adapter.
        adapter.add(chatbot.Chatbot(), Ice.Identity(name="greeter"))

        # Start dispatching requests.
        adapter.activate()

        # Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
        await communicator.shutdownCompleted()


if __name__ == "__main__":
    asyncio.run(main())
