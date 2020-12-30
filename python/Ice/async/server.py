#!/usr/bin/env python
#
# Copyright (c) ZeroC, Inc. All rights reserved.
#

import asyncio
import functools
import signal
import sys
import Ice

Ice.loadSlice('Hello.ice')
import Demo


class HelloI(Demo.Hello):
    def __init__(self, loop):
        self.loop = loop

    def sayHello(self, delay, current):
        if delay == 0:
            print("Hello World!")
        else:
            # return the concurrent future to Ice
            return asyncio.run_coroutine_threadsafe(self.slowSayHello(delay), self.loop)

    def shutdown(self, current):
        current.adapter.getCommunicator().shutdown()

    # helper coroutine function
    async def slowSayHello(self, delay):
        print(f"Sleeping {delay / 1000}s")
        await asyncio.sleep(delay=delay / 1000)
        print("(Delayed) Hello World!")


async def main():
    # Ice.initialize returns an initialized Ice communicator; the communicator is destroyed once it goes out of scope.
    with Ice.initialize(sys.argv, "config.server") as communicator:

        # The communicator initialization removes all Ice-related arguments from argv.
        if len(sys.argv) > 1:
            print(sys.argv[0] + ": too many arguments")
            return 1

        loop = asyncio.get_running_loop()

        # Install a signal handler to initiate communicator shutdown on Ctrl+C.
        loop.add_signal_handler(signal.SIGINT, communicator.shutdown)

        adapter = communicator.createObjectAdapter("Hello")
        adapter.add(HelloI(loop), Ice.stringToIdentity("hello"))
        adapter.activate()

        # Wait for communicator shutdown in a separate thread.
        await loop.run_in_executor(None, communicator.waitForShutdown)
        print("shutting down")
        return 0

sys.exit(asyncio.run(main()))
