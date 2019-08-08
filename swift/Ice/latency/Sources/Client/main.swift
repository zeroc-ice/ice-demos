//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Dispatch
import Ice
import PromiseKit

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

        guard let ping = try checkedCast(prx: communicator.propertyToProxy("Ping.Proxy")!, type: PingPrx.self) else {
            print("invalid proxy")
            return 1
        }

        let start = DispatchTime.now()
        let repetitions = 100_000
        print("pinging server \(repetitions) times (this may take a while)")
        for _ in 0 ..< repetitions {
            try ping.ice_ping()
        }

        let total = Double(DispatchTime.now().uptimeNanoseconds - start.uptimeNanoseconds) / 1_000_000
        let perPing = total / Double(repetitions)

        print("time for \(repetitions) pings: \(total)ms")
        print("time per ping: \(perPing)ms")
        return 0
    } catch {
        print("Error: \(error)")
        return 1
    }
}

exit(run())
