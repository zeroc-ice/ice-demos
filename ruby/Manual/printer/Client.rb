#!/usr/bin/env ruby
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

require 'Ice'
Ice::loadSlice('Printer.ice')

begin
    communicator = Ice::initialize(ARGV)
    base = communicator.stringToProxy("SimplePrinter:default -h localhost -p 10000")
    printer = Demo::PrinterPrx::checkedCast(base)
    if not printer
        raise "Invalid proxy"
    end

    printer.printString("Hello World!")
ensure
    if defined? communicator and communicator != nil
        communicator.destroy()
    end
end
