#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************
import signal
import sys
import Ice
import asyncio

Ice.loadSlice('Calculator.ice')
import Demo


def run(communicator):
    calculator = Demo.CalculatorPrx.checkedCast(communicator.propertyToProxy('Calculator.Proxy'))
    if not calculator:
        print("invalid proxy")
        sys.exit(1)

    # Calculate 10 - 4 with an asynchronous call that returns a future object
    print("10 minus 4 is ", calculator.subtractAsync(10, 4).result())

    def handleDivideFuture(future):
        try:
            # Since divideAsync has output parameters, the result is a list
            result = future.result()
            print("13 / 5 is", result[0], "with a remainder of", result[1])
        except Demo.DivideByZeroException:
            print("You cannot divide by 0")

    # Calculate 13 / 5 with asynchronous futures
    fut2 = calculator.divideAsync(13, 5)
    # Continue the operation with a callback function that is run when 'divideAsync' completes
    fut2.add_done_callback(handleDivideFuture)
    # Wait until the future has been fully completed
    fut2.result()

    # Same with 13 / 0
    fut3 = calculator.divideAsync(13, 0)
    fut3.add_done_callback(handleDivideFuture)
    try:
        fut3.result()
    except Exception:
        # Ignored, already caught by 'handleDivideFuture'
        pass

    # Have the calculator find the hypotenuse of a triangle with side lengths of 6 and 8 using the
    # Pythagorean theorem and chained futures
    try:
        side1 = calculator.squareAsync(6)
        side2 = calculator.squareAsync(8)
        sideSum = calculator.addAsync(side1.result(), side2.result())
        hypotenuse = calculator.squareRootAsync(sideSum.result())
        print("The hypotenuse of a triangle with side lengths of 6 and 8 is", hypotenuse.result())
    except Demo.NegativeRootException:
        print("You cannot take the square root of a negative number")

    loop = asyncio.get_event_loop()

    async def doSubtractAsync(x, subtrahend):
        result = await Ice.wrap_future(calculator.subtractAsync(x, subtrahend))
        print(x, "minus", subtrahend, "is", result)

    # Runs 'doSubtractAsync' until it's completed
    loop.run_until_complete(doSubtractAsync(10, 4))

    async def doDivideAsync(numerator, denominator):
        try:
            result = await Ice.wrap_future(calculator.divideAsync(numerator, denominator))
            print(numerator, "/", denominator, "is", result[0], "with a remainder of", result[1])
        except Demo.DivideByZeroException:
            print("You cannot divide by 0")

    loop.run_until_complete(doDivideAsync(13, 5))

    async def doHypotenuseAsync(x, y):
        # Combines multiple futures into one, which executes the underlying futures concurrently when run
        squareFuture = asyncio.gather(Ice.wrap_future(calculator.squareAsync(x)),
                                      Ice.wrap_future(calculator.squareAsync(y)))
        # gather returns all the future's results in a list
        sumFuture = Ice.wrap_future(calculator.addAsync(*(await squareFuture)))
        hypotenuseFuture = Ice.wrap_future(calculator.squareRootAsync(await sumFuture))
        try:
            print("The hypotenuse of a triangle with side lengths of", x, "and", y, "is", await hypotenuseFuture)
        except Demo.NegativeRootException:
            print("You cannot take the square root of a negative number")

    loop.run_until_complete(doHypotenuseAsync(6, 8))

    loop.close()

    calculator.shutdown()


#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv, "config.client") as communicator:

    #
    # Install a signal handler to destroy the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, handler: communicator.destroy())

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)

    run(communicator)
