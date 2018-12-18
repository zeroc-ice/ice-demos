#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import Ice

slice_dir = Ice.getSliceDir()
if not slice_dir:
    print(sys.argv[0] + ': Slice directory not found.')
    sys.exit(1)

Ice.loadSlice("'-I" + slice_dir + "' Callback.ice")
import Demo


class CallbackReceiverI(Demo.CallbackReceiver):
    def callback(self, num, current):
        print("received callback #{0}".format(num))


def run(communicator):
    server = Demo.CallbackSenderPrx.checkedCast(communicator.propertyToProxy('CallbackSender.Proxy'))
    if not server:
        print("invalid proxy")
        sys.exit(1)

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


#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv, "config.client") as communicator:

    #
    # Install a signal handler to shutdown the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, frame: communicator.shutdown())
    if hasattr(signal, 'SIGBREAK'):
        signal.signal(signal.SIGBREAK, lambda signum, frame: communicator.shutdown())

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)

    run(communicator)
