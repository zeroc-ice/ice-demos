#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import getpass
import sys

import Glacier2
import Ice

# Slice module VisitorCenter in Greeter.ice maps to Python module VisitorCenter.
import VisitorCenter


async def main():
    # Create an Ice communicator. We'll use this communicator to create proxies, and manage outgoing connections. We
    # enable asyncio support by passing the current event loop to initialize.
    async with Ice.Communicator(sys.argv, eventLoop=asyncio.get_running_loop()) as communicator:
        # Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is
        # derived from the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
        router = Glacier2.RouterPrx(communicator, "Glacier2/router:tcp -h localhost -p 4063")

        # Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
        # username/password combination. This call establishes a network connection to the Glacier2 router; the
        # lifetime of the session is the same as the lifetime of the connection.
        session = await router.createSessionAsync(getpass.getuser(), "password")

        # The proxy returned by createSession is None because we did not configure a SessionManager on the Glacier2
        # router.
        assert session is None

        # Create a proxy to the Greeter object in the server behind the Glacier2 router. Typically, the client cannot
        # connect directly to this server because it's on an unreachable network.
        greeter = VisitorCenter.GreeterPrx(communicator, "greeter:tcp -h localhost -p 4061")

        # Configure the proxy to route requests using the Glacier2 router.
        greeter = greeter.ice_router(router)

        # Send a request to the remote object and get the response.
        greeting = await greeter.greetAsync(getpass.getuser())
        print(greeting)

        # Send a second request to observe the effect in the Glacier2 router log.
        greeting = await greeter.greetAsync("alice")
        print(greeting)


if __name__ == "__main__":
    asyncio.run(main())
