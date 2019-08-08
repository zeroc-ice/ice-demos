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
        let communicator = try Ice.initialize(args: &args, configFile: "config.client")
        defer {
            communicator.destroy()
        }

        guard args.count == 1 else {
            print("too many arguments")
            return 1
        }

        //
        // Get the hello proxy. We configure the proxy to not cache the
        // server connection with the proxy and to disable the locator
        // cache. With this configuration, the IceGrid locator will be
        // queried for each invocation on the proxy and the invocation
        // will be sent over the server connection matching the returned
        // endpoints.
        //
        guard let base = try communicator.stringToProxy("hello")?
            .ice_connectionCached(false)
            .ice_locatorCacheTimeout(0),
            let hello = try checkedCast(prx: base, type: HelloPrx.self) else {
            print("invalid proxy")
            return 1
        }

        repeat {
            print("enter the number of iterations: ", terminator: "")

            guard let l1 = readLine(strippingNewline: true), l1 != "x" else {
                return 0
            }

            let count = Int(l1) ?? 10

            print("enter the delay between each greetings (in ms): ", terminator: "")
            guard let l2 = readLine(strippingNewline: true), l2 != "x" else {
                return 0
            }

            let delay = max(0, Int(l2) ?? 500)

            for _ in 0 ..< count {
                print(try hello.getGreeting())
                Thread.sleep(forTimeInterval: TimeInterval(delay) / 1000)
            }
        } while true
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
