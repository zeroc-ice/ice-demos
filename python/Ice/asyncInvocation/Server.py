#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import math, signal, sys, Ice

Ice.loadSlice('Calculator.ice')
import Demo

class CalculatorI(Demo.Calculator):
    async def add(self, x, y, current):
        return x + y

    async def subtract(self, x, subtrahend, current):
        return x - subtrahend

    async def divide(self, numerator, denominator, current):
        if(denominator == 0):
            raise Demo.DivideByZeroException()
        return (numerator // denominator, numerator % denominator)

    async def square(self, x, current):
        return x * x

    async def squareRoot(self, x, current):
        if(x < 0):
            raise Demo.NegativeRootException()
        return math.sqrt(x)

    async def shutdown(self, current):
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
        print(sys.argv[0] + " : too many arguments")
        sys.exit(1)

    adapter = communicator.createObjectAdapter("Calculator")
    adapter.add(CalculatorI(), Ice.stringToIdentity("calculator"))
    adapter.activate()
    communicator.waitForShutdown()
sys.exit(0)