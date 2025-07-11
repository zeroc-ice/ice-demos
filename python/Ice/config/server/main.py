#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import sys

import chatbot
import Ice


def main():
    # Create Ice properties from the contents of the config.server file in the current working directory.
    initData = Ice.InitializationData()
    initData.properties = Ice.createProperties()
    initData.properties.load("config.server")

    # Create an Ice communicator. We'll use this communicator to create an object adapter.
    # The communicator gets its properties from initData.properties; Ice.* properties and other reserved properties set
    # in sys.argv override these properties.
    with Ice.initialize(sys.argv, initData=initData) as communicator:
        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapter("GreeterAdapter")

        # Register the Chatbot servant with the adapter.
        adapter.add(chatbot.Chatbot(), Ice.Identity(name="greeter"))

        # Start dispatching requests.
        adapter.activate()

        # Ice.Trace.Network=1 or greater shows on which interface(s) and port(s) the server is listening.

        try:
            # Wait until communicator.shutdown() is called, which never occurs in this demo.
            communicator.waitForShutdown()
        except KeyboardInterrupt:
            print("Caught Ctrl+C, exiting...")


if __name__ == "__main__":
    main()
