#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import sys
import Ice
from numpy import random
import Demo


class MatrixI(Demo.Matrix):
    def fetchData(self, current):
        axisLength = random.randint(1, 5)

        order = Demo.MatrixType.RowMajor if random.choice([True, False]) else Demo.MatrixType.ColumnMajor

        elements = random.uniform(low=-5, high=5, size=(axisLength * random.randint(1, 5)))
        print(type(elements))
        return Demo.MatrixData(elements, order, axisLength)


def main():
    # Create an Ice communicator to initialize the Ice runtime. The communicator is destroyed at the end of the with block.
    with Ice.initialize(sys.argv) as communicator:

        # Create an object adapter that listens for incoming requests and dispatches them to servants.
        adapter = communicator.createObjectAdapterWithEndpoints("MatrixAdapter", "tcp -p 4061")
        adapter.add(MatrixI(), Ice.stringToIdentity("matrix"))
        adapter.activate()
        print("Listening on port 4061...")

        try:
            # Wait until communicator.shutdown() is called, which never occurs in this demo.
            communicator.waitForShutdown()
        except KeyboardInterrupt:
            print("Caught Ctrl+C, exiting...")

if __name__ == "__main__":
    main()