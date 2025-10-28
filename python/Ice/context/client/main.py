#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import getpass
import sys

import Ice

# Slice module VisitorCenter in Greeter.ice maps to Python module VisitorCenter.
from VisitorCenter import GreeterPrx


async def main():
    initData = Ice.InitializationData()
    initData.properties = Ice.Properties(sys.argv)

    # Set the Ice.ImplicitContext property to "Shared" before initializing the communicator.
    # This is only necessary for the implicit context API (see below).
    initData.properties.setProperty("Ice.ImplicitContext", "Shared")

    # Configure the communicator to use asyncio.
    initData.eventLoopAdapter = Ice.asyncio.EventLoopAdapter(asyncio.get_running_loop())

    # Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    async with Ice.Communicator(initData=initData) as communicator:
        # Create a Greeter proxy. If you run the server on a different computer, replace localhost in the string below
        # with the server's hostname or IP address.
        greeter = GreeterPrx(communicator, "greeter:tcp -h localhost -p 4061")

        # Send a request to the remote object and get the response. We request a French greeting by setting the context
        # parameter.
        greeting = await greeter.greetAsync(getpass.getuser(), context={"language": "fr"})
        print(greeting)

        # Do it again, this time by setting the context on the proxy.
        greeterEs = greeter.ice_context({"language": "es"})

        greeting = await greeterEs.greetAsync("alice")
        print(greeting)

        # One more time, this time with an implicit context set on the communicator.
        implicitContext = communicator.getImplicitContext()
        assert implicitContext is not None
        implicitContext.put("language", "de")
        greeting = await greeter.greetAsync("bob")
        print(greeting)


if __name__ == "__main__":
    asyncio.run(main())
