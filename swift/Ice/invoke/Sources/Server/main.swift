//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

// Automatically flush stdout
setbuf(__stdoutp, nil)

func run() -> Int32 {
    do {
        var args = CommandLine.arguments
        let communicator = try Ice.initialize(args: &args, configFile: "config.server")
        defer {
            communicator.destroy()
        }

        guard args.count == 1 else {
            print("too many arguments")
            return 1
        }

        let adapter = try communicator.createObjectAdapter("Printer")
        try adapter.add(servant: Ice.BlobjectDisp(PrinterI()), id: Ice.stringToIdentity("printer"))
        try adapter.activate()
        communicator.waitForShutdown()
        return 0
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
