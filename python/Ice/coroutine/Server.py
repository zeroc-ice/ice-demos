#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import math, sys, asyncio, requests, concurrent.futures, Ice

Ice.loadSlice('Fetcher.ice')
import Demo

class FetcherI(Demo.Fetcher):
    def __init__(self, loop):
        self.loop = loop

    async def fetch(self, url, current):
        try:
            # run_in_executor is a coroutine that dispatches the given function in an executor
            response = await self.loop.run_in_executor(None, requests.get, url)
        except Exception as ex:
            raise Demo.FetchException(str(ex))
        return str(response.content)

    async def shutdown(self, current):
        current.adapter.getCommunicator().shutdown()
        self.loop.call_soon_threadsafe(self.loop.stop)

class Server(Ice.Application):
    def interruptCallback(self, sig):
        self.loop.call_soon_threadsafe(self.loop.stop)
        self.communicator().shutdown()

    def run(self, args):

        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        self.loop = asyncio.get_event_loop()
        self.callbackOnInterrupt()

        adapter = self.communicator().createObjectAdapter("Fetcher")
        adapter.add(FetcherI(self.loop), Ice.stringToIdentity("fetcher"))
        adapter.activate()

        self.loop.run_forever()

        self.communicator().waitForShutdown()

        self.loop.close()

        return 0

sys.stdout.flush()
app = Server()
sys.exit(app.main(sys.argv, "config.server"))
