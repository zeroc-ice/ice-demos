#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import Ice

Ice.loadSlice('Context.ice')
import Demo


class ContextI(Demo.Context):
    def call(self, current):
        sys.stdout.write("Type = ")
        if 'type' in current.ctx:
            sys.stdout.write(current.ctx['type'])
        else:
            sys.stdout.write('None')
        sys.stdout.write("\n")

    def shutdown(self, current):
        current.adapter.getCommunicator().shutdown()


#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
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

    adapter = communicator.createObjectAdapter("Context")
    adapter.add(ContextI(), Ice.stringToIdentity("context"))
    adapter.activate()
    communicator.waitForShutdown()
