//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

// Automatically flush stdout
setbuf(__stdoutp, nil)

func run() -> Int32 {
    do {
        var args = CommandLine.arguments
        let communicator = try Ice.initialize(args: &args, configFile: "config.server")
        defer {
            communicator.destroy()
        }

        guard args.count == 1 else {
            print("too many arguments")
            return 1
        }

        let authenticator = AuthenticatorI()
        let authenticatorAdapter = try communicator.createObjectAdapter("Authenticator")
        try authenticatorAdapter.add(servant: AuthenticatorDisp(authenticator),
                                     id: Ice.stringToIdentity("authenticator"))
        try authenticatorAdapter.activate()

        let securedOperations: Set = ["setTemp", "shutdown"]

        let interceptor = InterceptorI(dispatcher: ThermostatDisp(ThermostatI()), authenticator: authenticator,
                                       securedOperations: securedOperations)
        let thermostatAdapter = try communicator.createObjectAdapter("Thermostat")
        try thermostatAdapter.add(servant: interceptor, id: Ice.stringToIdentity("thermostat"))
        try thermostatAdapter.activate()

        communicator.waitForShutdown()
        return 0
    } catch {
        print("Error: \(error)\n")
    }
    return 1
}

exit(run())
