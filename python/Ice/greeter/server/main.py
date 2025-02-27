#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import chatbot
import sys

# Create an Ice communicator to initialize the Ice runtime. The communicator is destroyed at the end of the with block.
with Ice.initialize(sys.argv) as communicator:
    # Create an object adapter that listens for incoming requests and dispatches them to servants.
    adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061")

    # Register the Chatbot servant with the adapter.
    adapter.add(chatbot.Chatbot(), Ice.Util.stringToIdentity("greeter"))

    # Start dispatching requests.
    adapter.activate()
    print("Listening on port 4061...")

    try:
        # Wait until communicator.shutdown() is called, which never occurs in this demo.
        communicator.waitForShutdown()
    except KeyboardInterrupt:
        print("Caught Ctrl+C, exiting...")
