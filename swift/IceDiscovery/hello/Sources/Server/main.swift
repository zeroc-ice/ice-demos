//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice
import Foundation

class HelloI: Hello {
    func sayHello(delay: Int32, current: Ice.Current) throws {
        if delay > 0 {
            Thread.sleep(forTimeInterval: TimeInterval(delay) / 1000)
        }
        print("Hello World!")
    }

    func shutdown(current: Ice.Current) throws {
        print("Shutting down...")
        if let adapter = current.adapter {
            adapter.getCommunicator().shutdown()
        }
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
        try adapter.add(servant: HelloI(), id: Ice.stringToIdentity("hello"))
        try adapter.activate()

        communicator.waitForShutdown()
    } catch {
        print("Error: \(error)\n")
        return 1
    }
    return 0
}

exit(run())
