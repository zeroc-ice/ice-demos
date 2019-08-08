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
        """)
}

enum Option: String {
    case twoway = "t"
    case oneway = "o"
    case batchOneway = "O"
    case datagram = "d"
    case batchDatagram = "D"
    case flushBatchRequests = "f"
    case timeout = "T"
    case delay = "P"
    case switchSecure = "S"
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

        guard let base = try communicator.stringToProxy("hello"),
            var twoway = try checkedCast(prx: base.ice_twoway().ice_secure(false), type: HelloPrx.self) else {
            print("invalid proxy")
            return 1
        }

        var oneway = twoway.ice_oneway()
        var batchOneway = twoway.ice_batchOneway()
        var datagram = twoway.ice_datagram()
        var batchDatagram = twoway.ice_batchDatagram()

        menu()

        var exit = false
        var secure = false
        var timeout = Int32(-1)
        var delay = Int32(0)
        repeat {
            do {
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
                case .twoway:
                    try twoway.sayHello(delay)
                case .oneway:
                    try oneway.sayHello(delay)
                case .batchOneway:
                    try batchOneway.sayHello(delay)
                case .datagram:
                    if secure {
                        print("secure datagrams are not supported")
                    } else {
                        try datagram.sayHello(delay)
                    }
                case .batchDatagram:
                    if secure {
                        print("secure datagrams are not supported")
                    } else {
                        try batchDatagram.sayHello(delay)
                    }
                case .flushBatchRequests:
                    try batchOneway.ice_flushBatchRequests()
                    if !secure {
                        try batchDatagram.ice_flushBatchRequests()
                    }
                case .timeout:
                    timeout = timeout == -1 ? 2000 : -1

                    twoway = twoway.ice_invocationTimeout(timeout)
                    oneway = oneway.ice_invocationTimeout(timeout)
                    batchOneway = batchOneway.ice_invocationTimeout(timeout)

                    if timeout == -1 {
                        print("timeout is now switched off")
                    } else {
                        print("timeout is now set to 2000ms")
                    }
                case .delay:
                    delay = delay == 0 ? 2500 : 0

                    if delay == 0 {
                        print("server delay is now deactivated")
                    } else {
                        print("server delay is now set to 2500ms")
                    }
                case .switchSecure:
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
                case .shutdown:
                    try twoway.shutdown()
                case .exit:
                    exit = true
                case .help:
                    menu()
                }
            } catch let ex as Ice.Exception {
                print(ex)
            }
        } while !exit
        return 0
    } catch {
        print("Error: \(error)")
        return 1
    }
}

exit(run())
