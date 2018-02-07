#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import Ice, sys
from numpy import random

Ice.loadSlice('Matrix.ice')
Ice.updateModules()
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

class Server(Ice.Application):
    def run(self, args):
        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        adapter = self.communicator().createObjectAdapter("Matrix")
        adapter.add(MatrixI(), Ice.stringToIdentity("matrix"))
        adapter.activate()

        self.communicator().waitForShutdown()
        return 0

app = Server()
sys.exit(app.main(sys.argv, "config.server"))
