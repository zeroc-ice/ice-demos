#!/usr/bin/env ruby
#
# Copyright (c) ZeroC, Inc. All rights reserved.
#

require 'Ice'

Ice::loadSlice('Hello.ice')

Ice::initialize(ARGV) do |communicator|
    hello = Demo::HelloPrx::checkedCast(communicator.stringToProxy("hello:default -h localhost -p 10000"))
    hello.sayHello()
end
