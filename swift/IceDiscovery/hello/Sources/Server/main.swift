//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

// Automatically flush stdout
setbuf(__stdoutp, nil)

class HelloI: Hello {
    func sayHello(delay: Int32, current _: Ice.Current) throws {
        if delay > 0 {
            Thread.sleep(forTimeInterval: TimeInterval(delay) / 1000)
        }
        print("Hello World!")
    }

    func shutdown(current: Ice.Current) throws {
        print("Shutting down...")
        current.adapter!.getCommunicator().shutdown()
    }
}

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

        let adapter = try communicator.createObjectAdapter("Hello")
        try adapter.add(servant: HelloDisp(HelloI()), id: Ice.stringToIdentity("hello"))
        try adapter.activate()

        communicator.waitForShutdown()
        return 0
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
