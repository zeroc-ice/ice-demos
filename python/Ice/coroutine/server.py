#!/usr/bin/env python
#
# Copyright (c) ZeroC, Inc. All rights reserved.
#

import concurrent.futures
import signal
import sys
import Ice

Ice.loadSlice('--all FibonacciBackEnd.ice')
import Demo


# The actual recursive fibonacci implementation. It's written in Python so it executes with the GIL locked.
def fibonacci(n):
    if n <= 0:
        raise Demo.InvalidValue(f"cannot compute fibonacci number for {n!r}")
    elif n == 1:
        return 0
    elif n == 2:
        return 1
    else:
        return fibonacci(n - 2) + fibonacci(n - 1)


# The servant for the back-end. It would typically live in a separate back-end server.
class FibonacciBackEndI(Demo.Fibonacci):
    def __init__(self, executor):
        self._executor = executor

    def compute(self, n, current):
        return self._executor.submit(fibonacci, n)


# The servant for Slice interface Fibonacci
# It's thread-safe because the object adapter hosting this servant is configured to use a single thread created by the
# underlying Ice C++ runtime.
class FibonacciI(Demo.Fibonacci):
    def __init__(self, proxy):
        self._proxy = proxy
        self._cache = dict()

    async def compute(self, n, current):
        if n in self._cache:
            return self._cache[n]
        else:
            # can raise InvalidValue
            result = await self._proxy.computeAsync(n)
            self._cache[n] = result
            return result

    def clearCache(self, current):
        self._cache.clear()

    def shutdown(self, current):
        # initiate communicator shutdown
        current.adapter.getCommunicator().shutdown()


if __name__ == "__main__":
    with Ice.initialize(sys.argv, "config.server") as communicator, concurrent.futures.ProcessPoolExecutor() as exec:

        if len(sys.argv) > 1:
            print(sys.argv[0] + ": too many arguments")
            sys.exit(1)

        signal.signal(signal.SIGINT, lambda signum, frame: communicator.shutdown())

        adapter = communicator.createObjectAdapter("Fibonacci")

        # We use a process pool to execute the Python CPU-intensive code in separate processes.
        proxy = Demo.FibonacciBackEndPrx.uncheckedCast(adapter.addWithUUID(FibonacciBackEndI(exec)))

        adapter.add(FibonacciI(proxy), Ice.stringToIdentity("Fibonacci"))  # front-end
        adapter.activate()
        communicator.waitForShutdown()
