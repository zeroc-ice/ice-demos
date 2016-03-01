#!/usr/bin/env ruby
# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

require 'Ice'
Ice::loadSlice('Printer.ice')

status = 0
ic = nil
begin
    ic = Ice::initialize(ARGV)
    base = ic.stringToProxy("SimplePrinter:default -h localhost -p 10000")
    printer = Demo::PrinterPrx::checkedCast(base)
    if not printer
        raise "Invalid proxy"
    end

    printer.printString("Hello World!")
rescue
    puts $!
    puts $!.backtrace.join("\n")
    status = 1
end

if ic
    # Clean up
    begin
        ic.destroy()
    rescue
        puts $!
        puts $!.backtrace.join("\n")
        status = 1
    end
end

exit(status)
