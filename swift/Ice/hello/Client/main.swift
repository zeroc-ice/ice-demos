//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

func menu() {
    print(
        """
        usage:
            t: send greeting as twoway
            o: send greeting as oneway
            O: send greeting as batch oneway
            d: send greeting as datagram
            D: send greeting as batch datagram
            f: flush all batch requests
            T: set a timeout
            P: set a server delay
            S: switch secure mode on/off
            s: shutdown server
            x: exit
        ?: help
        """);
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

        guard let base = try communicator.propertyToProxy("Hello.Proxy"),
              var twoway = try checkedCast(prx: base.ice_twoway().ice_secure(false), type: HelloPrx.self) else {
            print("invalid proxy")
            return 1
        }

        var oneway = twoway.ice_oneway()
        var batchOneway = twoway.ice_batchOneway()
        var datagram = twoway.ice_datagram()
        var batchDatagram = twoway.ice_batchDatagram()
        
        menu()

        var secure = false
        var timeout = Int32(-1)
        var delay = Int32(0)
        var line = ""
        repeat {
            
            print("==> ", terminator: "")
            guard let l = readLine(strippingNewline: true) else {
                return 0
            }
            line = l
            
            switch line {
            case "t":
                try twoway.sayHello(delay)
            case "o":
                try oneway.sayHello(delay)
            case "O":
                try batchOneway.sayHello(delay)
            case "d":
                if secure {
                    print("secure datagrams are not supported")
                } else {
                    try datagram.sayHello(delay)
                }
            case "D":
                if secure {
                    print("secure datagrams are not supported")
                } else {
                    try batchDatagram.sayHello(delay)
                }
            case "f":
                try batchOneway.ice_flushBatchRequests()
                if !secure {
                    try batchDatagram.ice_flushBatchRequests()
                }
            case "T":
                timeout = timeout == -1 ? 2000 : -1

                twoway = twoway.ice_invocationTimeout(timeout);
                oneway = oneway.ice_invocationTimeout(timeout);
                batchOneway = batchOneway.ice_invocationTimeout(timeout);
                
                if timeout == -1 {
                    print("timeout is now switched off")
                } else {
                    print("timeout is now set to 2000ms")
                }
            case "P":
                delay = delay == 0 ? 2500 : 0
                
                if delay == 0 {
                    print("server delay is now deactivated")
                } else {
                    print("server delay is now set to 2500ms")
                }
            case "S":
                secure = !secure
                
                twoway = twoway.ice_secure(secure)
                oneway = oneway.ice_secure(secure)
                batchOneway = batchOneway.ice_secure(secure)
                datagram = datagram.ice_secure(secure)
                batchDatagram = batchDatagram.ice_secure(secure)
                
                if secure {
                    print("secure mode is now on")
                } else {
                    print("secure mode is now off")
                }
            case "s":
                try twoway.shutdown()
            case "x":
                break
            case "?":
                menu()
            default:
                print("unknown command `\(line)'")
                menu()
            }
        } while line != "x"
    } catch {
        print("Error: \(error)")
        return 1
    }
    return 0
}

exit(run())
