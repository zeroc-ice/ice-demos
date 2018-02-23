#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal, sys, Ice

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
    # signal.signal must be called within the same scope as the communicator to catch CtrlC
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
sys.exit(0)
