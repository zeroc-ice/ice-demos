//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice
import PromiseKit

// Automatically flush stdout
setbuf(__stdoutp, nil)

class HelloI: Hello {
    var done = false
    var serialQueue = DispatchQueue(label: "com.zeroc.demo.ice.async.server")

    func sayHelloAsync(delay: Int32, current _: Current) -> Promise<Void> {
        return Promise { seal in
            if delay == 0 {
                print("Hello World!")
                seal.fulfill(())
            } else {
                serialQueue.async {
                    guard self.done == false else {
                        seal.reject(RequestCanceledException())
                        return
                    }
                    self.serialQueue.asyncAfter(deadline: .now() + .milliseconds(Int(delay))) {
                        guard self.done == false else {
                            seal.reject(RequestCanceledException())
                            return
                        }
                        print("Belated Hello World!")
                        seal.fulfill(())
                    }
                }
            }
        }
    }

    func shutdown(current: Current) throws {
        serialQueue.async {
            print("Shutting down...")
            self.done = true
            current.adapter!.getCommunicator().shutdown()
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
        try adapter.add(servant: HelloDisp(HelloI()), id: Ice.stringToIdentity("hello"))
        try adapter.activate()

        communicator.waitForShutdown()

    } catch {
        print("Error: \(error)\n")
        return 1
    }
    return 0
}

exit(run())
