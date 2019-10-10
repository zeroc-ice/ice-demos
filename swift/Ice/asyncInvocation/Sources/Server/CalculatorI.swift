//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice

class CalculatorI: Calculator {
    func add(x: Int32, y: Int32, current _: Ice.Current) throws -> Int32 {
        return x + y
    }

    func subtract(x: Int32, subtrahend: Int32, current _: Ice.Current) throws -> Int32 {
        return x - subtrahend
    }

    func divide(numerator: Int32,
                denominator: Int32,
                current _: Ice.Current) throws -> (returnValue: Int32, remainder: Int32) {
        guard denominator != 0 else {
            throw DivideByZeroException()
        }
        return (numerator / denominator, numerator % denominator)
    }

    func square(x: Int32, current _: Ice.Current) throws -> Int32 {
        return x * x
    }

    func squareRoot(x: Int32, current _: Ice.Current) throws -> Double {
        guard x >= 0 else {
            throw NegativeRootException()
        }
        return Double(x).squareRoot()
    }

    func shutdown(current: Ice.Current) {
        current.adapter!.getCommunicator().shutdown()
    }
}
