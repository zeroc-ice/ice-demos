#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import sys

import Ice
from Filesystem import DirectoryPrx, FilePrx
from m_directory import MDirectory
from m_file import MFile


def main():
    # Create an Ice communicator. We'll use this communicator to create an object adapter.
    with Ice.initialize(sys.argv) as communicator:
        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("Filesystem", "tcp -p 4061")

        # Create the root directory servant (with name "/"), and add this servant to the adapter.
        root = MDirectory("/")
        adapter.add(root, Ice.Identity(name="RootDir"))

        # Create a file called "README", add this servant to the adapter,
        # and add the corresponding proxy to the root directory.
        file = MFile("README")
        file.write_direct(["This file system contains a collection of poetry."])
        root.add_child(FilePrx.uncheckedCast(adapter.addWithUUID(file)))

        # Create a directory called "Coleridge", add this servant to the adapter,
        # and add the corresponding proxy to the root directory.
        coleridge = MDirectory("Coleridge")
        root.add_child(DirectoryPrx.uncheckedCast(adapter.addWithUUID(coleridge)))

        # Create a file called "Kubla_Khan", add this servant to the adapter,
        # and add the corresponding proxy to the Coleridge directory.
        file = MFile("Kubla_Khan")
        file.write_direct(
            [
                "In Xanadu did Kubla Khan",
                "A stately pleasure-dome decree:",
                "Where Alph, the sacred river, ran",
                "Through caverns measureless to man",
                "Down to a sunless sea.",
            ]
        )
        coleridge.add_child(FilePrx.uncheckedCast(adapter.addWithUUID(file)))

        # Start dispatching requests.
        adapter.activate()
        print("Listening on port 4061...")

        try:
            # Wait until communicator.shutdown() is called,
            # which never occurs in this demo.
            communicator.waitForShutdown()
        except KeyboardInterrupt:
            print("Caught Ctrl+C, exiting...")


if __name__ == "__main__":
    main()
