#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, traceback, time, Ice

Ice.loadSlice('Context.ice')
Ice.updateModules()
import Demo

class ContextI(Demo.Context):
    def call(self, current=None):
        sys.stdout.write("Type = ")
        if 'type' in current.ctx:
            sys.stdout.write(current.ctx['type'])
        else:
            sys.stdout.write('None')
        sys.stdout.write("\n")

    def shutdown(self, current=None):
        current.adapter.getCommunicator().shutdown()

class Server(Ice.Application):
    def run(self, args):
        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        adapter = self.communicator().createObjectAdapter("Context")
        adapter.add(ContextI(), Ice.stringToIdentity("context"))
        adapter.activate()
        self.communicator().waitForShutdown()
        return 0

sys.stdout.flush()
app = Server()
sys.exit(app.main(sys.argv, "config.server"))
