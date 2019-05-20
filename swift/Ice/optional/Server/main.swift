//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice
import Foundation

func run() -> Int32 {
    do {
        var args = CommandLine.arguments
        let communicator = try Ice.initialize(args: &args, configFile: "config.server")
        defer {
            communicator.destroy()
        }

        guard args.count == 1 else {
            print("too many arguments\n");
            return 1
        }

        let adapter = try communicator.createObjectAdapter("ContactDB")
        try adapter.add(servant: ContactDBI(), id: Ice.stringToIdentity("contactdb"))
        try adapter.activate()

        communicator.waitForShutdown()
        return 0
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
