//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

class CallbackSenderI: CallbackSender {
    var clients: [CallbackReceiverPrx] = []
    var num: Int32 = 0

    func addClient(receiver: CallbackReceiverPrx?, current: Ice.Current) throws {
        if let client = receiver {
            DispatchQueue.global(qos: .background).async(flags: .barrier) {
                print("adding client `\(Ice.identityToString(id: client.ice_getIdentity()))'")
                self.clients.append(client.ice_fixed(current.con!))
            }
        }
    }

    func dispatch() {
        if clients.count > 0 {
            num += 1
            for c in clients {
                c.callbackAsync(num).catch { e in
                    DispatchQueue.global(qos: .background).async(flags: .barrier) {
                        precondition(e is Ice.Exception)
                        print("removing client \(Ice.identityToString(id: c.ice_getIdentity())):\n:\(e)")
                        self.clients.removeAll { $0 == c }
                    }
                }
            }
        }
    }
}

func run() -> Int32 {
    do {
        //
        // using statement - communicator is automatically destroyed
        // at the end of this statement
        //
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
        let sender = CallbackSenderI()
        try adapter.add(servant: CallbackSenderDisp(sender), id: Ice.stringToIdentity("sender"))
        try adapter.activate()

        Timer.scheduledTimer(withTimeInterval: TimeInterval(2.0), repeats: true) { _ in
            DispatchQueue.global(qos: .background).async(flags: .barrier) {
                sender.dispatch()
            }
        }

        RunLoop.main.run()
    } catch {
        print("Error: \(error)\n")
        return 1
    }
    return 0
}

exit(run())
