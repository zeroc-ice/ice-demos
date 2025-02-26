#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import chatbot
import asyncio


async def main():

    loop = asyncio.get_event_loop()

    def coroutineExecutor(coroutine):
        """
        A custom coroutine executor that runs the coroutine in the main asyncio event loop.
        """

        # We use run_coroutine_threadsafe because the executor is called by an Ice thread.
        return asyncio.run_coroutine_threadsafe(coroutine, loop)

    # Create an initialization data structure that specifies the coroutine executor.
    initData = Ice.InitializationData()
    initData.coroutineExecutor = coroutineExecutor

    # Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
    with Ice.initialize(initData) as communicator:

        #print("Communicator initialized", communicator.getCoroutineExecutor())
        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061")

        # Register the Chatbot servant with the adapter.
        adapter.add(chatbot.Chatbot(), Ice.Util.stringToIdentity("greeter"))

        # Start dispatching requests.
        adapter.activate()
        print("Listening on port 4061...")

        try:
            # Wait until the event completes, which only occurs when it is canceled by Ctrl+C.
            exit_event = asyncio.Event()
            await exit_event.wait()
        except asyncio.exceptions.CancelledError:
            print("Caught Ctrl+C, exiting...")

asyncio.run(main())
