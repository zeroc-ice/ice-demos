#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import getpass
import sys

# Slice module VisitorCenter in Greeter.ice maps to Python module VisitorCenter.
import VisitorCenter

async def main():
    # Create an Ice communicator. We'll use this communicator to create proxies, and manage outgoing connections. We
    # enable asyncio support by passing the current event loop to initialize.
    async with Ice.initialize(sys.argv, eventLoop=asyncio.get_running_loop()) as communicator:

        # Set the default locator of the new communicator. It's the address of the Locator hosted by our IceGrid
        # registry. You can also set this proxy with the Ice.Default.Locator property.
        communicator.setDefaultLocator(Ice.LocatorPrx(communicator, "IceGrid/Locator:tcp -h localhost -p 4061"))

        # Create a proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no addressing
        # information, also known as a well-known proxy. It's specified by the <object> element in the IceGrid XML file.
        # The IceGrid registry resolves this well-known proxy and returns the actual address (endpoint) of the server to
        # this client.
        greeter = VisitorCenter.GreeterPrx(communicator, "greeter")

        # Send a request to the remote object and get the response.
        greeting = await greeter.greetAsync(getpass.getuser())
        print(greeting)

if __name__ == "__main__":
    asyncio.run(main())
