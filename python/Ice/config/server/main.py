#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import chatbot
import sys

def main():
    # Create an Ice communicator. We'll use this communicator to create an object adapter. The communicator gets its
    # configuration properties from file config.server, in the server's current working directory. The communicator
    # initialization also parses the command-line options to find and set additional properties.
    with Ice.initialize(sys.argv, configFile="config.server") as communicator:
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
