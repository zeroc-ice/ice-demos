#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, Ice

slice_dir = Ice.getSliceDir()
if not slice_dir:
    print(sys.argv[0] + ': Slice directory not found.')
    sys.exit(1)

Ice.loadSlice("'-I" + slice_dir + "' Callback.ice")
import Demo

class CallbackReceiverI(Demo.CallbackReceiver):
    def callback(self, num, current):
        print("received callback #" + str(num))

class Client(Ice.Application):

    def run(self, args):
        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        server = Demo.CallbackSenderPrx.checkedCast(self.communicator().propertyToProxy('CallbackSender.Proxy'))
        if not server:
            print(self.appName() + ": invalid proxy")
            return 1

        #
        # Create an object adapter with no name and no endpoints for receiving callbacks
        # over bidirectional connections.
        #
        adapter = self.communicator().createObjectAdapter("")

        #
        # Register the callback receiver servant with the object adapter and activate
        # the adapter.
        #
        proxy = Demo.CallbackReceiverPrx.uncheckedCast(adapter.addWithUUID(CallbackReceiverI()))
        adapter.activate()

        #
        # Associate the object adapter with the bidirectional connection.
        #
        server.ice_getConnection().setAdapter(adapter)

        #
        # Provide the proxy of the callback receiver object to the server and wait for
        # shutdown.
        #
        server.addClient(proxy)
        self.communicator().waitForShutdown()
        return 0

app = Client()
sys.exit(app.main(sys.argv, "config.client"))
