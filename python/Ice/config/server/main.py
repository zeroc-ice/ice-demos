#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import sys

import chatbot
import Ice


def main():
    # Load the contents of the config.server file into a Properties object.
    configFileProperties = Ice.createProperties()
    configFileProperties.load("config.server")

    # Create a Properties object from the command line arguments and the config file properties; Ice.* properties and
    # other reserved properties set in sys.argv augment or override the config file properties.
    properties = Ice.createProperties(sys.argv, configFileProperties)

    # Create an Ice communicator. We'll use this communicator to create an object adapter.
    # The communicator gets its properties from the properties object.
    with Ice.initialize(initData=Ice.InitializationData(properties=properties)) as communicator:
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
