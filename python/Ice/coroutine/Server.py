#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, asyncio, urllib.parse, os, Ice

Ice.loadSlice('Fetcher.ice')
import Demo

class FetcherI(Demo.Fetcher):
    def __init__(self, loop):
        self.loop = loop

    async def fetch(self, url, current):
        headers = ''
        try:
            #
            # Parse the URL to determine how the connection should be opened.
            #
            r = urllib.parse.urlsplit(url)
            if r.scheme == 'https':
                con = asyncio.open_connection(r.hostname, 443, loop=self.loop, ssl=True)
            else:
                con = asyncio.open_connection(r.hostname, 80, loop=self.loop)

            #
            # Wait for the connection to complete.
            #
            reader, writer = await con

            #
            # Submit a query.
            #
            query = ('HEAD {path} HTTP/1.0\r\n'
                     'Host: {hostname}\r\n'
                     '\r\n').format(path=r.path or '/', hostname=r.hostname)
            writer.write(query.encode('latin-1'))

            #
            # Read all of the headers.
            #
            while True:
                line = await reader.readline()
                if not line:
                    break
                line = line.decode('latin1').rstrip()
                if line:
                    if headers:
                        headers = headers + os.linesep + line
                    else:
                        headers = line

            writer.close()
        except Exception as ex:
            raise Demo.FetchException(str(ex))
        return headers

    async def shutdown(self, current):
        current.adapter.getCommunicator().shutdown()
        self.loop.call_soon_threadsafe(self.loop.stop)

class Server(Ice.Application):
    def interruptCallback(self, sig):
        #
        # Schedule a call to stop the loop. This causes run_forever() to return.
        #
        self.loop.call_soon_threadsafe(self.loop.stop)
        self.communicator().shutdown()

    def run(self, args):

        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        self.loop = asyncio.get_event_loop()

        #
        # This causes interruptCallback to be called on a signal.
        #
        self.callbackOnInterrupt()

        adapter = self.communicator().createObjectAdapter("Fetcher")
        adapter.add(FetcherI(self.loop), Ice.stringToIdentity("fetcher"))
        adapter.activate()

        #
        # Run the asyncio event loop until stop() is called.
        #
        self.loop.run_forever()

        self.communicator().waitForShutdown()

        self.loop.close()

        return 0

sys.stdout.flush()
app = Server()
sys.exit(app.main(sys.argv, "config.server"))
