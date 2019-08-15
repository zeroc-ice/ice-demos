//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice
import IceGrid

enum selection: String {
    case greeting = "t"
    case shutdown = "s"
    case exit = "x"
    case help = "?"
}

func menu() {
    print(
        """
        usage:
            t: send greeting
            s: shutdown server
            x: exit
            ?: help
        """)
}

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
        var hello: HelloPrx!
        do {
            let base = try communicator.stringToProxy("hello")!
            hello = try checkedCast(prx: base, type: HelloPrx.self)
        } catch is Ice.NotRegisteredException {
            let base = try communicator.stringToProxy("DemoIceGrid/Query")!
            let query = try checkedCast(prx: base, type: QueryPrx.self)
            if let prx = try query?.findObjectByType("::Demo::Hello") {
                hello = try checkedCast(prx: prx, type: HelloPrx.self)
            }
        }
        guard hello != nil else {
            print("couldn't find a `::Demo::Hello' object")
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
            case .greeting:
                try hello.sayHello()
            case .shutdown:
                try hello.shutdown()
            case .exit:
                exit = true
            case .help:
                menu()
            }
        } while !exit
    } catch {
        print("Error: \(error)\n")
        return 1
    }
    return 0
}

exit(run())
