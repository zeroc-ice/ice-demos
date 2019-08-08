//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Glacier2
import Ice

// Automatically flush stdout
setbuf(__stdoutp, nil)

class CallbackReceiverI: CallbackReceiver {
    func callback(current _: Current) {
        print("received callback")
    }
}

enum Option: String {
    case twoway = "t"
    case oneway = "o"
    case batchOneway = "O"
    case flush = "f"
    case overrideContextField = "v"
    case fakeCategory = "F"
    case shutdown = "s"
    case exit = "x"
    case help = "?"
}

func menu() {
    print(
        """
        usage:
            t: invoke callback as twoway
            o: invoke callback as oneway
            O: invoke callback as batch oneway
            f: flush all batch requests
            v: set/reset override context field
            F: set/reset fake category
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
        guard let defaultRouter = communicator.getDefaultRouter(),
            let router = try checkedCast(prx: defaultRouter, type: Glacier2.RouterPrx.self) else {
                print("Default router not set")
            return 1
        }

        // Prompt user for authentication
        // Will continue to loop until a session is successfully created
        var session: SessionPrx?
        while session == nil {
            print("This demo accepts any user-id / password combination.")
            print("user id: ", terminator: "")
            let username = readLine(strippingNewline: true)!
            print("password: ", terminator: "")
            let password = readLine(strippingNewline: true)!

            // Create session
            do {
                session = try router.createSession(userId: username, password: password)
                break
            } catch {
                print("Cannot created session: \(error)")
            }
        }
        let acmTimeout = try router.getACMTimeout()
        guard let connection = router.ice_getCachedConnection() else { fatalError() }
        connection.setACM(timeout: acmTimeout, close: nil, heartbeat: .HeartbeatAlways)

        try connection.setCloseCallback { _ in
            print("The Glacier2 session has been destroyed.")
        }

        // The Glacier2 router routes bidirectional calls to objects in the client only when these objects have the
        // correct Glacier2-issued category. The purpose of the callbackReceiverFakeIdent is to demonstrate this.
        // The name that is provided as an arguement is not parsed by the server.
        let uuid = UUID()
        let callbackReceiverIdent = try Identity(name: uuid.uuidString, category: router.getCategoryForClient())
        let callbackReceiverFakeIdent = Identity(name: uuid.uuidString, category: "fake")

        guard let base = try communicator.propertyToProxy("Callback.Proxy"),
            let twoway = try checkedCast(prx: base, type: CallbackPrx.self)
        else {
            print("invalid proxy")
            return 1
        }

        let oneway = twoway.ice_oneway()
        let batchOneway = twoway.ice_batchOneway()
        let adapter = try communicator.createObjectAdapterWithRouter(name: "", rtr: router)
        try adapter.activate()
        try adapter.add(servant: CallbackReceiverDisp(CallbackReceiverI()), id: callbackReceiverIdent)
        // Callback will never be called for a fake identity
        try adapter.add(servant: CallbackReceiverDisp(CallbackReceiverI()), id: callbackReceiverFakeIdent)

        var twowayR = try uncheckedCast(prx: adapter.createProxy(callbackReceiverIdent), type: CallbackReceiverPrx.self)
        var onewayR = twowayR.ice_oneway()

        var override = ""
        var fake = false
        var exit = false

        menu()

        // Client REPL
        repeat {
            print("==> ", terminator: "")
            guard let line = readLine(strippingNewline: true) else {
                return 0
            }
            if let option = Option(rawValue: line) {
                switch option {
                case .twoway:
                    try twoway.initiateCallback(twowayR)
                case .oneway:
                    let context: Context? = override.isEmpty ? nil : ["_ovrd": override]
                    try oneway.initiateCallback(onewayR, context: context)
                case .batchOneway:
                    let context: Context = override.isEmpty ? ["_fwd": "O"] : ["_fwd": "O", "_ovrd": override]
                    try batchOneway.initiateCallback(onewayR, context: context)
                case .flush:
                    try batchOneway.ice_flushBatchRequests()
                case .overrideContextField:
                    override = override.isEmpty ? "some_value" : ""
                    print("override context field is now \(override)")
                case .fakeCategory:
                    fake = !fake
                    let ident = fake ? callbackReceiverFakeIdent : callbackReceiverIdent
                    twowayR = uncheckedCast(prx: twowayR.ice_identity(ident), type: CallbackReceiverPrx.self)
                    onewayR = uncheckedCast(prx: onewayR.ice_identity(ident), type: CallbackReceiverPrx.self)
                    print("callback receiver identity: \(identityToString(id: twowayR.ice_getIdentity()))")
                case .shutdown:
                    try twoway.shutdown()
                case .exit:
                    exit = true
                case .help:
                    menu()
                }
            } else {
                if !line.isEmpty { print("unknown command `\(line)`") }
            }
        } while !exit
        return 0
    } catch {
        print("Error \(error)")
        return 1
    }
}

exit(run())
