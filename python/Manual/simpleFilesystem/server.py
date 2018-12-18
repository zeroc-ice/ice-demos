#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import Ice

Ice.loadSlice('Filesystem.ice')
import Filesystem


class DirectoryI(Filesystem.Directory):
    def __init__(self, communicator, name, parent):
        self._name = name
        self._parent = parent
        self._contents = []
        #
        # Create an identity. The root directory has the fixed identity "RootDir"
        #
        self._id = Ice.Identity()
        if self._parent:
            self._id.name = Ice.generateUUID()
        else:
            self._id.name = "RootDir"

    #
    # Slice Node::name() operation
    #
    def name(self, current):
        return self._name

    #
    # Slice Directory::list() operation
    #
    def list(self, current):
        return self._contents

    #
    # addChild is called by the child in order to add
    # itself to the _contents member of the parent
    #
    def addChild(self, child):
        self._contents.append(child)

    #
    # Add servant to ASM and Parent's _contents map.
    #
    def activate(self, a):
        thisNode = Filesystem.DirectoryPrx.uncheckedCast(a.add(self, self._id))
        if self._parent:
            self._parent.addChild(thisNode)


class FileI(Filesystem.File):
    def __init__(self, communicator, name, parent):
        self._name = name
        self._parent = parent
        self.lines = []

        assert(self._parent is not None)
        #
        # Create an identity
        #
        self._id = Ice.Identity()
        self._id.name = Ice.generateUUID()

    #
    # Slice Node::name() operation
    #
    def name(self, current):
        return self._name

    #
    # Slice File::read() operation
    #
    def read(self, current):
        return self._lines

    #
    # Slice File::write() operation
    #
    def write(self, text, current):
        self._lines = text

    #
    # Add servant to ASM and Parent's _contents map.
    #
    def activate(self, a):
        thisNode = Filesystem.FilePrx.uncheckedCast(a.add(self, self._id))
        self._parent.addChild(thisNode)


def run(communicator):
    #
    # Create an object adapter
    #
    adapter = communicator.createObjectAdapterWithEndpoints(
        "SimpleFileSystem", "default -h localhost -p 10000")

    #
    # Create the root directory (with name "/" and no parent)
    #
    root = DirectoryI(communicator, "/", None)
    root.activate(adapter)

    #
    # Create a file called "README" in the root directory
    #
    file = FileI(communicator, "README", root)
    text = ["This file system contains a collection of poetry."]
    try:
        file.write(text, None)
    except Filesystem.GenericError as e:
        print(e.reason)
    file.activate(adapter)

    #
    # Create a directory called "Coleridge" in the root directory
    #
    coleridge = DirectoryI(communicator, "Coleridge", root)
    coleridge.activate(adapter)

    #
    # Create a file called "Kubla_Khan" in the Coleridge directory
    #
    file = FileI(communicator, "Kubla_Khan", coleridge)
    text = ["In Xanadu did Kubla Khan",
            "A stately pleasure-dome decree:",
            "Where Alph, the sacred river, ran",
            "Through caverns measureless to man",
            "Down to a sunless sea."]
    try:
        file.write(text, None)
    except Filesystem.GenericError as e:
        print(e.reason)
    file.activate(adapter)

    #
    # All objects are created, allow client requests now
    #
    adapter.activate()

    #
    # Wait until we are done
    #
    communicator.waitForShutdown()


#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv) as communicator:
    #
    # Install a signal handler to shutdown the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, frame: communicator.shutdown())
    if hasattr(signal, 'SIGBREAK'):
        signal.signal(signal.SIGBREAK, lambda signum, frame: communicator.shutdown())
    run(communicator)
