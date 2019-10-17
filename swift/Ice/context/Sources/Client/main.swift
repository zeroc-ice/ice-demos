//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

// Automatically flush stdout
setbuf(__stdoutp, nil)

func menu() {
    print(
        """
        usage:
            1: call with no request context
            2: call with explicit request context
            3: call with per-proxy request context
            4: call with implicit request context
            s: shutdown server
            x: exit
            ?: help
        """)
}

enum Option: String {
    case noRequestContext = "1"
    case explicitRequestContext = "2"
    case perProxyRequestContext = "3"
    case implicitRequestContext = "4"
    case shutdown = "s"
    case exit = "x"
    case help = "?"
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

        guard let base = try communicator.propertyToProxy("Context.Proxy"),
            let proxy = try checkedCast(prx: base, type: ContextPrx.self) else {
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

            guard let option = Option(rawValue: line) else {
                print("unknown command `\(line)'")
                menu()
                continue
            }

            switch option {
            case .noRequestContext:
                try proxy.call()
            case .explicitRequestContext:
                try proxy.call(context: ["type": "Explicit"])
            case .perProxyRequestContext:
                try proxy.ice_context(["type": "Per-Proxy"]).call()
            case .implicitRequestContext:
                let implicitContext = communicator.getImplicitContext()
                implicitContext.setContext(["type": "Implicit"])
                try proxy.call()
                implicitContext.setContext([:])
            case .shutdown:
                try proxy.shutdown()
            case .exit:
                exit = true
            case .help:
                menu()
            }
        } while !exit
        return 0
    } catch {
        print("Error: \(error)")
        return 1
    }
}

exit(run())
