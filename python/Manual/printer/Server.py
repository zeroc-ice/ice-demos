#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, traceback, Ice

Ice.loadSlice('Printer.ice')
import Demo

class PrinterI(Demo._PrinterDisp):
    def printString(self, s, current=None):
        print(s)

with Ice.initialize(sys.argv) as ic:
    adapter = ic.createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000")
    object = PrinterI()
    adapter.add(object, Ice.stringToIdentity("SimplePrinter"))
    adapter.activate()
    ic.waitForShutdown()
