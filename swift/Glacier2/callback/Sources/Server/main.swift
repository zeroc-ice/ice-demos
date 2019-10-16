//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

// Automatically flush stdout
setbuf(__stdoutp, nil)

class CallbackI: Callback {
    func initiateCallback(proxy: CallbackReceiverPrx?, current: Current) {
        do {
            let proxyName = current.adapter!.getCommunicator().proxyToString(proxy)
            print("initiating callback to: \(proxyName)")
            try proxy?.callback()
        } catch {
            print("Error \(error)")
        }
    }

    func shutdown(current: Current) {
        print("shutting down...")
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
        let adapter = try communicator.createObjectAdapter("Callback.Server")
        try adapter.add(servant: CallbackDisp(CallbackI()), id: stringToIdentity("callback"))
        try adapter.activate()
        communicator.waitForShutdown()
    } catch {
        print(error)
        return 1
    }
    return 0
}

exit(run())
