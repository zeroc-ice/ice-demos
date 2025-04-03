#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import chatbot
import sys

def main():
    # Configure the communicator to load the IceDiscovery plugin during initialization. This plugin installs a default
    # locator on the communicator.
    initData = Ice.InitializationData()
    initData.properties = Ice.createProperties(sys.argv)
    initData.properties.setProperty("Ice.Plugin.IceDiscovery", "IceDiscovery:createIceDiscovery")

    # Configure the object adapter GreeterAdapter. It must be an indirect object adapter (i.e., with an AdapterId
    # property); otherwise, the IceDiscovery plugin can't make it discoverable by IceDiscovery clients.
    initData.properties.setProperty("GreeterAdapter.AdapterId", "greeterAdapterId")

    # Configure the GreeterAdapter to listen on TCP with an OS-assigned port. We don't need a fixed port since the
    # clients discover this object adapter.
    initData.properties.setProperty("GreeterAdapter.Endpoints", "tcp")

    # Create an Ice communicator. We'll use this communicator to create an object adapter.
    with Ice.initialize(initData=initData) as communicator:
        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapter("GreeterAdapter")

        # Register the Chatbot servant with the adapter.
        adapter.add(chatbot.Chatbot(), Ice.stringToIdentity("greeter"))

        # Start dispatching requests. This method also registers the object adapter with the IceDiscovery plugin.
        adapter.activate()
        print("Listening...")

        try:
            # Wait until communicator.shutdown() is called, which never occurs in this demo.
            communicator.waitForShutdown()
        except KeyboardInterrupt:
            print("Caught Ctrl+C, exiting...")

if __name__ == "__main__":
    main()
