//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

func run() -> Int32 {
    do {
        var args = CommandLine.arguments
        let communicator = try Ice.initialize(args: &args, configFile: "config.server")
        defer {
            communicator.destroy()
        }

        guard args.count == 1 else {
            print("too many arguments\n")
            return 1
        }

        let adapter = try communicator.createObjectAdapter("Calculator")
        try adapter.add(servant: CalculatorDisp(CalculatorI()), id: Ice.stringToIdentity("calculator"))
        try adapter.activate()

        communicator.waitForShutdown()
    } catch {
        print("Error: \(error)\n")
        return 1
    }
    return 0
}

exit(run())
