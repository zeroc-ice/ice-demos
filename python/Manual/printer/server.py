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
with Ice.initialize(sys.argv) as communicator:

    #
    # Install a signal handler to shutdown the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, frame: communicator.shutdown())
    if hasattr(signal, 'SIGBREAK'):
        signal.signal(signal.SIGBREAK, lambda signum, frame: communicator.shutdown())
    adapter = communicator.createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000")
    adapter.add(PrinterI(), Ice.stringToIdentity("SimplePrinter"))
    adapter.activate()
    communicator.waitForShutdown()
