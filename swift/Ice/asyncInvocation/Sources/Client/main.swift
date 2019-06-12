//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice
import PromiseKit

func run() -> Int32 {
    do {
        var args = CommandLine.arguments
        let communicator = try Ice.initialize(args: &args, configFile: "config.client")
        defer {
            communicator.destroy()
        }

        guard args.count == 1 else {
            print("too many arguments")
            return 1
        }

        guard let base = try communicator.propertyToProxy("Calculator.Proxy"),
            let calculator = try checkedCast(prx: base, type: CalculatorPrx.self) else {
            print("invalid proxy")
            return 1
        }

        var status: Int32 = 0
        firstly {
            //
            // Calculate 10 - 4 with an asynchronous call which in this case returns a Promise<Int32>
            //
            calculator.subtractAsync(x: 10, subtrahend: 4)
        }.get { result in
            print("10 minus 4 is \(result)")
        }.then { _ in
            //
            // Calculate 13 / 5 asynchronously
            //
            calculator.divideAsync(numerator: 13, denominator: 5)
        }.get { r in
            // Since the divide operation has output parameters, the promise result is a tuple
            // with the return value and the output parameters
            print("13 / 5 is \(r.returnValue) with a remainder of \(r.remainder)")
        }.then { _ in
            //
            // Same with 13 / 0
            //
            calculator.divideAsync(numerator: 13, denominator: 0)
        }.get { r in
            // Since the divide operation has output parameters, the promise result is a tuple
            // with the return value and the output parameters
            print("13 / 0 is \(r.returnValue) with a remainder of \(r.remainder)")
        }.recover { error -> Promise<(returnValue: Int32, remainder: Int32)> in
            guard error is DivideByZeroException else {
                throw error
            }
            print("You cannot divide by zero")
            return .value((returnValue: 0, remainder: 0))
        }.then { _ in
            //
            // Have the calculator find the hypotenuse of a triangle with side lengths of 6 and 8
            // using the Pythagorean theorem and chained asynchronous tasks.
            //
            when(fulfilled: calculator.squareAsync(6), calculator.squareAsync(8))
        }.then { side1, side2 in
            calculator.addAsync(x: side1, y: side2)
        }.then { sum in
            calculator.squareRootAsync(sum)
        }.get { hypotenuse in
            print("The hypotenuse of a triangle with side lengths of 6 and 8 is \(hypotenuse)")
        }.then { _ in
            calculator.shutdownAsync()
        }.catch { error in
            if error is DivideByZeroException {
                print("You cannot take the square root of a negative number")
            } else {
                print("Error: \(error)")
            }
            status = 1
        }.finally {
            communicator.destroy()
            exit(status)
        }
        RunLoop.main.run()
        return 0
    } catch {
        print("Error: \(error)")
        return 1
    }
}

exit(run())
