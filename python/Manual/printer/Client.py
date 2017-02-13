#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, Ice

Ice.loadSlice('Printer.ice')
import Demo

with Ice.initialize(sys.argv) as ic:
    base = ic.stringToProxy("SimplePrinter:default -h localhost -p 10000")
    printer = Demo.PrinterPrx.checkedCast(base)
    if not printer:
        raise RuntimeError("Invalid proxy")

    printer.printString("Hello World!")
