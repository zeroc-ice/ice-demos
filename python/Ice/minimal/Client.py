#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, traceback, Ice

Ice.loadSlice('Hello.ice')
import Demo

with Ice.initialize(sys.argv) as communicator:
    hello = Demo.HelloPrx.checkedCast(communicator.stringToProxy("hello:default -h localhost -p 10000"))
    hello.sayHello()
