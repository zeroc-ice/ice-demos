#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import Ice

Ice.loadSlice('Hello.ice')
import Demo


class HelloI(Demo.Hello):
    def __init__(self, name):
        self.name = name

    def sayHello(self, current):
        print(self.name + " says Hello World!")

    def shutdown(self, current):
        print(self.name + " shutting down...")
        current.adapter.getCommunicator().shutdown()


#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv) as communicator:

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

    properties = communicator.getProperties()
    adapter = communicator.createObjectAdapter("Hello")
    id = Ice.stringToIdentity(properties.getProperty("Identity"))
    adapter.add(HelloI(properties.getProperty("Ice.ProgramName")), id)
    adapter.activate()
    communicator.waitForShutdown()
