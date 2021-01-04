#!/usr/bin/env python
#
# Copyright (c) ZeroC, Inc. All rights reserved.
#

import asyncio
import functools
import sys
import Ice

Ice.loadSlice('Calculator.ice')
import Demo


async def main():
    with Ice.initialize(sys.argv, "config.client") as communicator:

        calculator = Demo.CalculatorPrx.checkedCast(communicator.propertyToProxy('Calculator.Proxy'))
        if not calculator:
            print("invalid proxy")
            return 1

        # Ice.wrap_future converts an Ice future into an awaitable asyncio future
        r = await Ice.wrap_future(calculator.subtractAsync(10, 4))
        print(f"10 minus 4 is {r}")

        # another way to do the same, this time with a blocking remote call (subtract - not subtractAsync) executed in
        # a separate executor thread
        loop = asyncio.get_running_loop()
        r = await loop.run_in_executor(None, functools.partial(calculator.subtract, 12, 3))
        print(f"12 minus 3 is {r}")

        q, r = await Ice.wrap_future(calculator.divideAsync(13, 5))
        print(f"13 / 5 is {q} with a remainder of {r}")

        try:
            await Ice.wrap_future(calculator.divideAsync(13, 0))
        except Demo.DivideByZeroException:
            print("You cannot divide by 0")

        side1Future = Ice.wrap_future(calculator.squareAsync(6))  # asyncio future
        side2Future = Ice.wrap_future(calculator.squareAsync(8))  # another asyncio future
        await asyncio.gather(side1Future, side2Future)  # wait for both calls to complete

        sideSum = await Ice.wrap_future(calculator.addAsync(side1Future.result(), side2Future.result()))
        hypotenuse = await Ice.wrap_future(calculator.squareRootAsync(sideSum))
        print(f"The hypotenuse of a triangle with side lengths of 6 and 8 is {hypotenuse}")

        await Ice.wrap_future(calculator.shutdownAsync())
        return 0

sys.exit(asyncio.run(main()))
