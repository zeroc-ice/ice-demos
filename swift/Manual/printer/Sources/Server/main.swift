//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

struct PrinterI: Printer {
    func printString(s: String, current _: Ice.Current) {
        print(s)
    }
}

func run() -> Int32 {
    do {
        let communicator = try Ice.initialize(CommandLine.arguments)
        defer {
            communicator.destroy()
        }

        let adapter = try communicator.createObjectAdapterWithEndpoints(name: "SimplePrinterAdapter",
                                                                        endpoints: "default -h localhost -p 10000")
        try adapter.add(servant: PrinterDisp(PrinterI()),
                        id: Ice.stringToIdentity("SimplePrinter"))
        try adapter.activate()

        communicator.waitForShutdown()
        return 0
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
