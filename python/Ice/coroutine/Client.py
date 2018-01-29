#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, Ice

Ice.loadSlice('Fetcher.ice')
import Demo

class Client(Ice.Application):

    def run(self, args):

        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        fetcher = Demo.FetcherPrx.checkedCast(self.communicator().propertyToProxy('Fetcher.Proxy'))
        if not fetcher:
            print("invalid proxy")
            return 1

        content = fetcher.fetch('http://zeroc.com')
        print("HTTP headers from zeroc.com:")
        print(content)

        fetcher.shutdown()
        return 0

app = Client()
sys.exit(app.main(sys.argv, "config.client"))
