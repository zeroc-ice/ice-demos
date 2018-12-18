#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import asyncio
import urllib.parse
import os
import Ice

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
        self.loop.call_soon_threadsafe(self.loop.stop)

#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv, "config.server") as communicator:

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)

    loop = asyncio.get_event_loop()

    #
    # Install a signal handler to stop asyncio loop and shutdown the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, frame: loop.call_soon_threadsafe(loop.stop))

    adapter = communicator.createObjectAdapter("Fetcher")
    adapter.add(FetcherI(loop), Ice.stringToIdentity("fetcher"))
    adapter.activate()

    #
    # Run the asyncio event loop until stop() is called.
    #
    loop.run_forever()
    loop.close()
