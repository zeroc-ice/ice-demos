#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import signal
import sys
import uuid

import chatbot
import Ice


async def main():
    initData = Ice.InitializationData()

    # Configure the communicator to use asyncio.
    loop = asyncio.get_running_loop()
    initData.eventLoopAdapter = Ice.asyncio.EventLoopAdapter(loop)

    initData.properties = Ice.Properties(sys.argv)

    # Configure the communicator to load the IceDiscovery plug-in during initialization. This plug-in installs a default
    # locator on the communicator.
    initData.properties.setProperty("Ice.Plugin.IceDiscovery", "1")

    # Configure the object adapter GreeterAdapter. It must be an indirect object adapter (i.e., with an AdapterId
    # property); otherwise, the IceDiscovery plug-in can't make it discoverable by IceDiscovery clients.
    # We also set the ReplicaGroupId property to "greeterPool" to enable replication.
    greeter_uuid = str(uuid.uuid4())
    initData.properties.setProperty("GreeterAdapter.AdapterId", f"greeter-{greeter_uuid}")
    initData.properties.setProperty("GreeterAdapter.ReplicaGroupId", "greeterPool")

    # Configure the GreeterAdapter to listen on TCP with an OS-assigned port. We don't need a fixed port since the
    # clients discover this object adapter.
    initData.properties.setProperty("GreeterAdapter.Endpoints", "tcp")

    # Create an Ice communicator. We'll use this communicator to create an object adapter.
    async with Ice.Communicator(initData=initData) as communicator:
        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapter("GreeterAdapter")

        # Register the Chatbot servant with the adapter. Here, well-known object "greeter" is replicated across all
        # server instances.
        adapter.add(chatbot.Chatbot(greeter_uuid[0:4]), Ice.Identity(name="greeter"))

        # Start dispatching requests. This method also registers the object adapter with the IceDiscovery plug-in.
        adapter.activate()
        print("Listening...")

        # Shutdown the communicator when the user presses Ctrl+C.
        signal.signal(signal.SIGINT, lambda _signum, _frame: communicator.shutdown())

        # Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
        await communicator.shutdownCompleted()


if __name__ == "__main__":
    asyncio.run(main())
