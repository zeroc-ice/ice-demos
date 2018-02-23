#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, Ice

Ice.loadSlice('Printer.ice')
import Demo

class PrinterI(Demo.Printer):
    def printString(self, s, current):
        print(s)

with Ice.initialize(sys.argv) as ic:
    adapter = ic.createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000")
    object = PrinterI()
    adapter.add(object, Ice.stringToIdentity("SimplePrinter"))
    adapter.activate()
    ic.waitForShutdown()
