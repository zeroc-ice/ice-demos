//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.Ice.asyncInvocation"]
module Demo
{
    exception DivideByZeroException
    {
    }

    exception NegativeRootException
    {
    }

    interface Calculator
    {
        idempotent int add(int x, int y);

        idempotent int subtract(int x, int subtrahend);

        idempotent int divide(int numerator, int denominator, out int remainder) throws DivideByZeroException;

        idempotent int square(int x);

        idempotent double squareRoot(int x) throws NegativeRootException;

        void shutdown();
    }
}
