#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import getpass
import sys

# Slice module VisitorCenter in Greeter.ice maps to Python module VisitorCenter.
import VisitorCenter

# Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
with Ice.initialize(sys.argv) as communicator:

    # GreeterPrx is a class generated by the Slice compiler. We create a proxy from a communicator and a "stringified
    # proxy" with the address of the target object.
    # If you run the server on a different computer, replace localhost in the string below with the server's hostname
    # or IP address.
    greeter = VisitorCenter.GreeterPrx(communicator, "greeter:tcp -h localhost -p 4061")

    # Send a request to the remote object and get the response.
    greeting = greeter.greet(getpass.getuser())
    print(greeting)

    # Send another request to the remote object, this time with greetAsync. greetAsync returns a future immediately.
    f = greeter.greetAsync("alice")
    greeting = f.result()
    print(greeting)
