//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import PromiseKit
import Ice

//
// Run PromiseKit completion in .global queue so we can use the main
// queue for wait f
//
PromiseKit.conf.Q.map = .global()
PromiseKit.conf.Q.return = .global()
PromiseKit.conf.logHandler = { _ in }

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
        
        //
        // Calculate 10 - 4 with an asynchronous call which in this case returns a Promise<Int32>
        //
        try Promise<Void> { seal in
            firstly {
                calculator.subtractAsync(x: 10, subtrahend: 4)
            }.done { result in
                print("10 minus 4 is \(result)")
            }.catch { error in
                seal.reject(error)
            }
        }.wait()

        //
        // Calculate 13 / 5 asynchronously
        //
        try Promise<Void> { seal in
            calculator.divideAsync(numerator: 13, denominator: 5).done { r in
                
                // Since the divide operation has output parameters, the promise result is a tuple
                // with the return value and the output parameters
                print("13 / 5 is \(r.returnValue) with a remainder of \(r.remainder)")
                seal.fulfill(())
            }.catch { error in
                if error is DivideByZeroException {
                    // Don't let this exception stop the rest of the program's execution
                    print("You cannot divide by zero")
                    seal.fulfill(())
                } else {
                    // Allow this exception to terminate the program's execution
                    seal.reject(error)
                }
            }
        }.wait()
        
        //
        // Same with 13 / 0
        //
        try Promise<Void> { seal in
            calculator.divideAsync(numerator: 13, denominator: 0).done { r in
                
                // Since the divide operation has output parameters, the promise result is a tuple
                // with the return value and the output parameters
                print("13 / 0 is \(r.returnValue) with a remainder of \(r.remainder)")
                seal.fulfill(())
            }.catch { error in
                if error is DivideByZeroException {
                    // Don't let this exception stop the rest of the program's execution
                    print("You cannot divide by zero")
                    seal.fulfill(())
                } else {
                    // Allow this exception to terminate the program's execution
                    seal.reject(error)
                }
            }
        }.wait()
        
        //
        // Have the calculator find the hypotenuse of a triangle with side lengths of 6 and 8
        // using the Pythagorean theorem and chained asynchronous tasks.
        //
        try Promise<Void> { seal in
            firstly {
                when(fulfilled: calculator.squareAsync(6), calculator.squareAsync(8))
            }.then { side1, side2 in
                calculator.addAsync(x: side1, y: side2)
            }.then { sum in
                calculator.squareRootAsync(sum)
            }.done { hypotenuse in
                print("The hypotenuse of a triangle with side lengths of 6 and 8 is \(hypotenuse)")
                seal.fulfill(())
            }.catch {error in
                if error is DivideByZeroException {
                    print("You cannot take the square root of a negative number")
                    seal.fulfill(())
                } else {
                    seal.reject(error)
                }
            }
        }.wait()

        try calculator.shutdown()

        return 0
    } catch {
        print("Error: \(error)")
        return 1
    }
}

exit(run())

