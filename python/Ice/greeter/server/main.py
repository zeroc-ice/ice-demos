#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import chatbot
import signal
import sys
import threading

exit_event = threading.Event()
signal.signal(signal.SIGINT, lambda sig, frame: exit_event.set())

# Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
with Ice.initialize(sys.argv) as communicator:
    # Create an object adapter that listens for incoming requests and dispatches them to servants.
    adapter = communicator.createObjectAdapterWithEndpoints("GreeterAdapter", "tcp -p 4061")

    adapter.add(chatbot.Chatbot(), Ice.Util.stringToIdentity("greeter"))

    # Start dispatching requests.
    adapter.activate()
    print("Listening on port 4061...")

    # Wait until the user presses Ctrl+C.
    exit_event.wait()
    print("Caught Ctrl+C, exiting...")
