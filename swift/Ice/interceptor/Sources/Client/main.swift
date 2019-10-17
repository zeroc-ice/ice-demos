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
        1: gets the current temperature
        2: sets the temperature
        3: gets a new token from the server
        4: releases the currently held token
        s: shutdown server
        x: exit
        ?: help
        """
    )
}

enum Option: String {
    case getTemp = "1"
    case setTemp = "2"
    case getToken = "3"
    case releaseToken = "4"
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

        guard let thermostatBase = try communicator.propertyToProxy("Thermostat.Proxy"),
              let thermostat = try checkedCast(prx: thermostatBase, type: ThermostatPrx.self) else {
            print("invalid thermostat proxy")
            return 1
        }

        guard let authenticatorBase = try communicator.propertyToProxy("Authenticator.Proxy"),
              let authenticator = try checkedCast(prx: authenticatorBase, type: AuthenticatorPrx.self) else {
            print("invalid authenticator proxy")
            return 1
        }

        menu()

        var exit = false
        repeat {
            print("\n==> ", terminator: "")

            guard let line = readLine(strippingNewline: true) else {
                return 0
            }
            guard let option = Option(rawValue: line) else {
                print("unknown command `\(line)'")
                menu()
                continue
            }

            switch option {
            case .getTemp:
                print("Current temperature is \(try thermostat.getTemp())")
            case .setTemp:
                do {
                    print("Enter desired temperature: ")
                    guard let line = readLine(strippingNewline: true), let temp = Float(line) else {
                        print("Provided temperature is not a parsable float.")
                        continue
                    }
                    try thermostat.setTemp(temp)
                    print("New temperature is \(try thermostat.getTemp())")
                } catch is TokenExpiredException {
                    print("Failed to set temperature. Token expired!")
                } catch is AuthorizationException {
                    print("Failed to set temperature. Access denied!")
                }
            case .getToken:
                //
                // Request an access token from the server's authentication object.
                //
                let token = try authenticator.getToken()
                print("Successfully retrieved access token: \"\(token)\"")
                //
                // Add the access token to the communicator's context, so it will be
                // sent along with every request made through it.
                //
                _ = communicator.getImplicitContext().put(key: "accessToken", value: token)
            case .releaseToken:
                let context = communicator.getImplicitContext()
                if context.containsKey("accessToken") {
                    _ = context.remove("accessToken")
                } else {
                    print("There is no access token to release")
                }
            case .shutdown:
                do {
                    try thermostat.shutdown()
                } catch is TokenExpiredException {
                    print("Failed to shutdown thermostat. Token expired!")
                } catch is AuthorizationException {
                    print("Failed to shutdown thermostat. Access denied!")
                }
            case .exit:
                exit = true
            case .help:
                menu()
            }
        } while !exit
        return 0
    } catch {
        print("Error \(error)")
    }
    return 1
}

exit(run())
