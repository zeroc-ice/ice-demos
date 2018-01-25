#!/usr/bin/env ruby
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

require 'Ice'

Ice::loadSlice('Hello.ice')

begin
    communicator = Ice::initialize(ARGV)
    hello = Demo::HelloPrx::checkedCast(communicator.stringToProxy("hello:default -h localhost -p 10000"))
    hello.sayHello()
ensure
    if defined? communicator and communicator != nil
        communicator.destroy()
    end
end
