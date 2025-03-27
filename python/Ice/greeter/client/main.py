#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import getpass
import sys

# Slice module VisitorCenter in Greeter.ice maps to Python module VisitorCenter.
import VisitorCenter

async def main():
    # Create an Ice communicator to initialize the Ice runtime. It will be automatically destroyed at the end of the
    # with block. We enable asyncio support by passing the current event loop to initialize.
    with Ice.initialize(sys.argv, eventLoop=asyncio.get_running_loop()) as communicator:

        # GreeterPrx is a class generated by the Slice compiler. We create a proxy from a communicator and a "stringified
        # proxy" with the address of the target object.
        # If you run the server on a different computer, replace localhost in the string below with the server's hostname
        # or IP address.
        greeter = VisitorCenter.GreeterPrx(communicator, "greeter:tcp -h localhost -p 4061")

        # Send a request to the remote object and get the response.
        greeting = await greeter.greetAsync(getpass.getuser())
        print(greeting)

asyncio.run(main())
