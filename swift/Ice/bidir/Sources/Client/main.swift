//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

// Automatically flush stdout
setbuf(__stdoutp, nil)

class CallbackReceiverI: CallbackReceiver {
    func callback(num: Int32, current _: Current) throws {
        print("received callback #\(num)")
    }
}

func run() -> Int32 {
    do {
        var args = CommandLine.arguments
        let communicator = try Ice.initialize(args: &args, configFile: "config.client")
        defer {
            communicator.destroy()
        }

        guard let base = try communicator.propertyToProxy("CallbackSender.Proxy"),
            let server = try checkedCast(prx: base, type: CallbackSenderPrx.self) else {
            print("invalid proxy")
            return 1
        }

        //
        // Create an object adapter with no name and no endpoints for receiving callbacks
        // over bidirectional connections.
        //
        let adapter = try communicator.createObjectAdapter("")

        //
        // Register the callback receiver servant with the object adapter
        //
        let proxy = try uncheckedCast(prx: adapter.addWithUUID(CallbackReceiverDisp(CallbackReceiverI())),
                                      type: CallbackReceiverPrx.self)

        //
        // Associate the object adapter with the bidirectional connection.
        //
        try server.ice_getConnection()!.setAdapter(adapter)

        //
        // Provide the proxy of the callback receiver object to the server and wait for
        // shutdown.
        //
        try server.addClient(proxy)
        communicator.waitForShutdown()

        return 0
    } catch {
        return 1
    }
}

exit(run())
