//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice
import PromiseKit

// Automatically flush stdout
setbuf(__stdoutp, nil)

// Use global queue to avoid blocking REPL which runs on main thread
PromiseKit.conf.Q.return = .global()

enum selection: String {
    case immediate = "i"
    case delayed = "d"
    case shutdown = "s"
    case exit = "x"
    case help = "?"
}

func menu() {
    print(
        """
        usage:
            i: send immediate greeting
            d: send delayed greeting
            s: shutdown server
            x: exit
            ?: help
        """)
}

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

        guard let base = try communicator.propertyToProxy("Hello.Proxy"),
            let hello = try checkedCast(prx: base, type: HelloPrx.self) else {
            print("invalid proxy")
            return 1
        }

        menu()

        var exit = false
        repeat {
            print("==> ", terminator: "")
            guard let line = readLine(strippingNewline: true) else {
                return 0
            }
            guard let choice = selection(rawValue: line) else {
                print("unknown command `\(line)'")
                menu()
                continue
            }
            switch choice {
            case .immediate:
                try hello.sayHello(0)
            case .delayed:
                firstly {
                    hello.sayHelloAsync(5000)
                }.catch { error in
                    print("sayHello AMI call failed:\n \(error)")
                }
            case .shutdown:
                try hello.shutdown()
            case .exit:
                exit = true
            case .help:
                menu()
            }
        } while !exit
    } catch {
        print("Error: \(error)")
        return 1
    }
    return 0
}

exit(run())
