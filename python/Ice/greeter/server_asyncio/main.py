#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import chatbot
import asyncio
import signal

async def main():

    loop = asyncio.get_event_loop()

    def coroutineExecutor(coroutine):
        """
        A custom coroutine executor that runs the coroutine in the main asyncio event loop.
        """

        # We use run_coroutine_threadsafe because the executor is called by an Ice thread.
        return asyncio.run_coroutine_threadsafe(coroutine, loop)

    # Create an initialization data structure that sets the coroutine executor. The coroutine executor is used by Ice
    # to run coroutines returned by servant dispatch methods in the main asyncio event loop.
    initData = Ice.InitializationData()
    initData.coroutineExecutor = coroutineExecutor

    # Create an Ice communicator to initialize the Ice runtime. The communicator is destroyed at the end of the with block.
    with Ice.initialize(initData) as communicator:

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
        #
        # Ice.wrap_future converts the Ice future returned by communicator.shutdownCompleted()
        # into an asyncio future, making it awaitable within the asyncio event loop.
        await Ice.wrap_future(communicator.shutdownCompleted())

asyncio.run(main())
