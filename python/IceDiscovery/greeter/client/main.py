#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import getpass
import sys

# Slice module VisitorCenter in Greeter.ice maps to Python module VisitorCenter.
import VisitorCenter

async def main():

    # Configure the communicator to load the IceDiscovery plugin during initialization. This plugin installs a default
    # locator on the communicator.
    initData = Ice.InitializationData()
    initData.properties = Ice.createProperties(sys.argv)
    initData.properties.setProperty("Ice.Plugin.IceDiscovery", "1")

    # Configure the Ice runtime to use asyncio.
    initData.eventLoopAdapter = Ice.asyncio.EventLoopAdapter(asyncio.get_running_loop())

    # Create an Ice communicator. We'll use this communicator to create proxies, and manage outgoing connections.
    async with Ice.initialize(initData=initData) as communicator:

        # Create a proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no
        # addressing information, also known as a well-known proxy. It's resolved by the default locator installed by
        # the IceDiscovery plugin.
        greeter = VisitorCenter.GreeterPrx(communicator, "greeter")

        # Send a request to the remote object and get the response.
        greeting = await greeter.greetAsync(getpass.getuser())
        print(greeting)

if __name__ == "__main__":
    asyncio.run(main())
