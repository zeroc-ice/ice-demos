#!/usr/bin/env ruby
# Copyright (c) ZeroC, Inc.

require 'etc'
require 'Ice'

# Load the code generated by the Slice compiler. The Slice module VisitorCenter maps to a Ruby module with the same
# name.
require_relative 'Greeter.rb'

# Create an Ice communicator to initialize the Ice runtime. The communicator is destroyed at the end of the block
# returned by Ice::initialize.
Ice::initialize(ARGV, "config.client") do |communicator|
    # GreeterPrx is a class generated by the Slice compiler. We create a proxy from a communicator and a
    # property found with the "Greeter.Proxy" specification with the address of the target object.
    # If you run the server on a different computer, replace localhost in the string below with the server's hostname
    # or IP address.
    properties = communicator.getProperties()
    greeter = VisitorCenter::GreeterPrx.new(communicator, properties.getProperty("Greeter.Proxy"))
    
    # Send a request to the remote object and get the response.
    greeting = greeter.greet(Etc.getlogin)

    puts greeting
end