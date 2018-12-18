#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys
import traceback
import Ice

Ice.loadSlice('Callback.ice')
import Demo


class CallbackI(Demo.Callback):
    def initiateCallback(self, proxy, current):
        print("initiating callback to: " + current.adapter.getCommunicator().proxyToString(proxy))
        try:
            proxy.callback()
        except Exception:
            traceback.print_exc()

    def shutdown(self, current):
        print("shutting down...")
        current.adapter.getCommunicator().shutdown()


class Server(Ice.Application):
    def run(self, args):
        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        adapter = self.communicator().createObjectAdapter("Callback.Server")
        adapter.add(CallbackI(), Ice.stringToIdentity("callback"))
        adapter.activate()
        self.communicator().waitForShutdown()
        return 0


app = Server()
sys.exit(app.main(sys.argv, "config.server"))
