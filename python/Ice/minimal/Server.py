#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, traceback, Ice

Ice.loadSlice('Hello.ice')
import Demo

class HelloI(Demo._HelloDisp):
    def sayHello(self, current=None):
        print("Hello World!")

with Ice.initialize(sys.argv) as communicator:
    adapter = communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000")
    adapter.add(HelloI(), Ice.stringToIdentity("hello"))
    adapter.activate()
    communicator.waitForShutdown()
