#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal, sys, Ice

Ice.loadSlice('Printer.ice')
import Demo

class PrinterI(Demo.Printer):
    def printString(self, s, current):
        print(s)

#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv) as ic:

    #
    # signal.signal must be called within the same scope as the communicator to catch CtrlC
    #
    signal.signal(signal.SIGINT, lambda signum, frame: ic.shutdown())
    adapter = ic.createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000")
    object = PrinterI()
    adapter.add(object, Ice.stringToIdentity("SimplePrinter"))
    adapter.activate()
    ic.waitForShutdown()
