#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal, sys, Ice

slice_dir = Ice.getSliceDir()
if not slice_dir:
    print(sys.argv[0] + ': Slice directory not found.')
    sys.exit(1)

Ice.loadSlice("'-I" + slice_dir + "' Callback.ice")
import Demo

class CallbackReceiverI(Demo.CallbackReceiver):
    def callback(self, num, current):
        print("received callback #" + str(num))

def run(communicator):
    server = Demo.CallbackSenderPrx.checkedCast(communicator.propertyToProxy('CallbackSender.Proxy'))
    if not server:
        print("invalid proxy")
        return 1

    #
    # Create an object adapter with no name and no endpoints for receiving callbacks
    # over bidirectional connections.
    #
    adapter = communicator.createObjectAdapter("")

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
    communicator.waitForShutdown()

    return 0

status = 0
#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv, "config.client") as communicator:

    #
    # signal.signal must be called within the same scope as the communicator to catch CtrlC
    #
    signal.signal(signal.SIGINT, lambda signum, frame: communicator.shutdown())

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        status = 1
    else:
        status = run(communicator)\

sys.exit(status)
