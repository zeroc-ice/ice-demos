//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice
import PromiseKit

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
            let twoway = try checkedCast(prx: base.ice_twoway().ice_secure(false), type: HelloPrx.self)
        else {
            print("invalid proxy")
            return 1
        }

        menu()

        var line = ""
        repeat {
            print("==> ", terminator: "")
            guard let l = readLine(strippingNewline: true) else {
                return 0
            }
            line = l
            guard let choice = selection(rawValue: line) else {
                print("unknown command `\(line)'")
                menu()
                return 0
            }
            switch choice {
            case .immediate:
                try twoway.sayHello(0)
            case .delayed:
                firstly {
                    twoway.sayHelloAsync(5000)
                }.catch { error in
                    print("sayHello AMI call failed:\n \(error)")
                }
            case .shutdown:
                try twoway.shutdown()
            case .exit:
                break
            case .help:
                menu()
            }
        } while line != "x"
    } catch {
        print("Error: \(error)")
        return 1
    }
    return 0
}

exit(run())
