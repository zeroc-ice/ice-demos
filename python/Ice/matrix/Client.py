#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import Ice, sys
import numpy as np

Ice.loadSlice('Matrix.ice')
import Demo

class Client(Ice.Application):

    def run(self, args):

        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        matrix = Demo.MatrixPrx.checkedCast(self.communicator().propertyToProxy('Matrix.Proxy'))
        if not matrix:
            print("invalid proxy")
            return 1

        # Fetch a matrix from the server
        data, major, axisLength = matrix.fetchData()

        # Numpy uses 'C' for row-major arrays, and 'F' for column-major
        if major == Demo.MatrixType.RowMajor:
            major = 'C'
        elif major == Demo.MatrixType.ColumnMajor:
            major = 'F'

        # Convert the data to a numpy-array
        data = np.asarray(data, order=major).reshape((axisLength, -1), order=major)
        # Convert the numpy-array into a numpy-matrix
        data = np.matrix(data)

        # Print the dimensions and elements of the matrix
        print("Recieved " + str(data.shape[0]) + " by " + str(data.shape[1]) + " matrix:")
        print(data)
        
        # Compute some properties of the matrix
        sum = data.sum()
        print("sum: " + str(sum))
        mean = data.mean()
        print("average: " + str(mean))
        std = data.std()
        print("standard deviation: " + str(std))

app = Client()
sys.exit(app.main(sys.argv, "config.client"))
