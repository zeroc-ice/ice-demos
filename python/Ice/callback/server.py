#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import traceback
import Ice

Ice.loadSlice('Callback.ice')
import Demo


class CallbackSenderI(Demo.CallbackSender):
    def initiateCallback(self, proxy, current):
        print("initiating callback")
        try:
            proxy.callback()
        except Exception:
            traceback.print_exc()

    def shutdown(self, current):
        print("Shutting down...")
        current.adapter.getCommunicator().shutdown()


#
# The Ice communicator is initlialized with Ice.initialize
# The communicator is destroyed once it goes out of scope of the with statement
#
with Ice.initialize(sys.argv, "config.server") as communicator:

    #
    # Install a signal handler to shutdown the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, frame: communicator.shutdown())

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)

    adapter = communicator.createObjectAdapter("Callback.Server")
    adapter.add(CallbackSenderI(), Ice.stringToIdentity("callbackSender"))
    adapter.activate()
    communicator.waitForShutdown()
