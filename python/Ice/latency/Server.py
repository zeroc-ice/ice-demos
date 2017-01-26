#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, traceback, Ice

Ice.loadSlice('Latency.ice')
import Demo

class Server(Ice.Application):
    def run(self, args):
        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        adapter = self.communicator().createObjectAdapter("Latency")
        adapter.add(Demo.Ping(), Ice.stringToIdentity("ping"))
        adapter.activate()
        self.communicator().waitForShutdown()
        return 0

app = Server()
sys.exit(app.main(sys.argv, "config.server"))
