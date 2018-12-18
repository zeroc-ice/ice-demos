#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import Ice
import threading

slice_dir = Ice.getSliceDir()
if not slice_dir:
    print(sys.argv[0] + ': Slice directory not found.')
    sys.exit(1)

Ice.loadSlice("'-I" + slice_dir + "' Props.ice")
import Demo

#
#  The servant implements the Slice interface Demo.Props
#


class PropsI(Demo.Props, Ice.PropertiesAdminUpdateCallback):
    def __init__(self):
        self.called = False
        self.m = threading.Condition()

    def getChanges(self, current=None):
        with self.m:
            #
            # Make sure that we have received the property updates before we
            # return the results.
            #
            while not self.called:
                self.m.wait()
            self.called = False
            return self.changes

    def shutdown(self, current=None):
        current.adapter.getCommunicator().shutdown()

    def updated(self, changes):
        with self.m:
            self.changes = changes
            self.called = True
            self.m.notify()


#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv, "config.server") as communicator:

    #
    # Install a signal handler to shutdown the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, handler: communicator.shutdown())

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)
    servant = PropsI()

    #
    # Retrieve the PropertiesAdmin facet and register the servant as the update callback.
    #
    admin = communicator.findAdminFacet("Properties")
    admin.addUpdateCallback(servant)

    adapter = communicator.createObjectAdapter("Props")
    adapter.add(servant, Ice.stringToIdentity("props"))
    adapter.activate()
    communicator.waitForShutdown()
