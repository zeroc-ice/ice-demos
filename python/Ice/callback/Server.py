#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, traceback, Ice

Ice.loadSlice('Callback.ice')
import Demo

class CallbackSenderI(Demo.CallbackSender):
    def initiateCallback(self, proxy, current=None):
        print("initiating callback")
        try:
            proxy.callback()
        except:
            traceback.print_exc()

    def shutdown(self, current=None):
        print("Shutting down...")
        try:
            current.adapter.getCommunicator().shutdown()
        except:
            traceback.print_exc()

class Server(Ice.Application):
    def run(self, args):
        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        adapter = self.communicator().createObjectAdapter("Callback.Server")
        adapter.add(CallbackSenderI(), Ice.stringToIdentity("callbackSender"))
        adapter.activate()
        self.communicator().waitForShutdown()
        return 0

app = Server()
sys.exit(app.main(sys.argv, "config.server"))
