#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import Ice
from numpy import random
Ice.loadSlice('Matrix.ice')
import Demo


class MatrixI(Demo.Matrix):
    def fetchData(self, current=None):
        axisLength = random.randint(1, 5)

        if random.choice([True, False]):
            type = Demo.MatrixType.RowMajor
        else:
            type = Demo.MatrixType.ColumnMajor

        elements = random.uniform(low=-5, high=5, size=(axisLength * random.randint(1, 5)))

        return Demo.MatrixData(elements, type, axisLength)


#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv, "config.server") as communicator:

    #
    # Install a signal handler to shutdown the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, frame: communicator.shutdown())

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)

    adapter = communicator.createObjectAdapter("Matrix")
    adapter.add(MatrixI(), Ice.stringToIdentity("matrix"))
    adapter.activate()
    communicator.waitForShutdown()
