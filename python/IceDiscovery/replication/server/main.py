#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import sys
import uuid

import chatbot
import Ice


def main():
    initData = Ice.InitializationData()
    initData.properties = Ice.createProperties(sys.argv)

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
    with Ice.initialize(initData=initData) as communicator:
        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapter("GreeterAdapter")

        # Register the Chatbot servant with the adapter. Here, well-known object "greeter" is replicated across all
        # server instances.
        adapter.add(chatbot.Chatbot(greeter_uuid[0:4]), Ice.Identity(name="greeter"))

        # Start dispatching requests. This method also registers the object adapter with the IceDiscovery plug-in.
        adapter.activate()
        print("Listening...")

        try:
            # Wait until communicator.shutdown() is called, which never occurs in this demo.
            communicator.waitForShutdown()
        except KeyboardInterrupt:
            print("Caught Ctrl+C, exiting...")


if __name__ == "__main__":
    main()
