#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import sys
import Ice

Ice.loadSlice('Hello.ice')
import Demo

#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv) as communicator:
    hello = Demo.HelloPrx.checkedCast(communicator.stringToProxy("hello:default -h localhost -p 10000"))
    hello.sayHello()
