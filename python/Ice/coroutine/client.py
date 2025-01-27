#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import signal
import sys
import time
import Ice

Ice.loadSlice('Fibonacci.ice')
import Demo


def run(communicator):
    fibonacci = Demo.FibonacciPrx.uncheckedCast(communicator.propertyToProxy('Fibonacci.Proxy'))

    try:
        v = fibonacci.compute(-1)
        assert False
    except Demo.InvalidValue:
        pass  # expected

    print("computing fibonacci number 35...")
    start = time.perf_counter()
    v = fibonacci.compute(35)
    printResult(v, start)

    print("computing fibonacci number 36...")
    start = time.perf_counter()
    v = fibonacci.compute(36)
    printResult(v, start)

    print("retrieving again fibonacci number 35...")
    start = time.perf_counter()
    v = fibonacci.compute(35)
    printResult(v, start)

    print("retrieving again fibonacci number 36...")
    start = time.perf_counter()
    v = fibonacci.compute(36)
    printResult(v, start)

    print("clearing cache")
    fibonacci.clearCache()

    # We can see from the log timestamps the numbers are indeed computed in parallel.
    print("computing fibonacci number 35 and 36 in parallel...")
    start = time.perf_counter()
    fa = fibonacci.computeAsync(35)
    fb = fibonacci.computeAsync(36)
    va = fa.result()
    vb = fb.result()
    printResult(f"{va} and {vb}", start)

    print("shutting down server")
    fibonacci.shutdown()


def printResult(result, start):
    print(f"result is {result}, took {time.perf_counter() - start:.2f}s")


# Ice.initialize returns an initialized Ice communicator, and this the communicator is destroyed once it goes out of
# scope.
with Ice.initialize(sys.argv, "config.client") as communicator:

    # Install a signal handler to destroy the communicator on Ctrl-C
    signal.signal(signal.SIGINT, lambda signum, handler: communicator.destroy())

    # The communicator initialization removes all Ice-related arguments from argv
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)

    run(communicator)
