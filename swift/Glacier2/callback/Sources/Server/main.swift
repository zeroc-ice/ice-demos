//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Glacier2
import Ice
import PromiseKit

class CallbackI: Callback {
    func initiateCallback(proxy: CallbackReceiverPrx?, current: Current) throws {
        do {
            let proxyName = current.adapter?.getCommunicator().proxyToString(proxy)
            print("Initiating callback to: \(String(describing: proxyName)))")
            try proxy?.callback()
        } catch {
            print("Error \(error)")
        }
    }

    func shutdown(current: Current) throws {
        print("Shutting down...")
        current.adapter?.getCommunicator().shutdown()
    }
}

var communicator: Communicator?

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
        let cb = CallbackI()
        try adapter.add(servant: cb, id: stringToIdentity("callback"))
        try adapter.activate()
        communicator.waitForShutdown()
    } catch {
        print(error)
        return 1
    }
    return 0
}

exit(run())
