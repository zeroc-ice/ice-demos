//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "Fibonacci.ice" // for InvalidValue

module Demo
{
    // A straight computation of fibonacci values (no cache).
    interface FibonacciBackEnd
    {
        idempotent int compute(int n) throws InvalidValue;
    }
}
