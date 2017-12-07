#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import math, sys, Ice;

Ice.loadSlice('Calculator.ice')
Ice.updateModules()
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

class Server(Ice.Application):
    def run(self, args):

        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        adapter = self.communicator().createObjectAdapter("Calculator")
        adapter.add(CalculatorI(), Ice.stringToIdentity("calculator"))

        adapter.activate()

        self.communicator().waitForShutdown()
        return 0

sys.stdout.flush()
app = Server()
sys.exit(app.main(sys.argv, "config.server"))
