//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Dispatch
import Ice
import PromiseKit

// Automatically flush stdout
setbuf(__stdoutp, nil)

func menu() {
    print(
        """
        usage:
            1: print string
            2: print string sequence
            3: print dictionary
            4: print enum
            5: print struct
            6: print struct sequence
            7: print class
            8: get values
            9: throw exception
            s: shutdown server
            x: exit
            ?: help
        """)
}

enum Option: String {
    case printString = "1"
    case printStringSeq = "2"
    case printDictionary = "3"
    case printEnum = "4"
    case printStruct = "5"
    case printStructSequence = "6"
    case printClass = "7"
    case getValues = "8"
    case throwException = "9"
    case shutdownServer = "s"
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

        guard let obj = try communicator.propertyToProxy("Printer.Proxy") else {
            print("invalid proxy")
            return 1
        }

        menu()

        var exit = false
        repeat {
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
            case .printString:
                //
                // Marshal the in parameter.
                //
                let outStream = Ice.OutputStream(communicator: communicator)
                outStream.startEncapsulation()
                outStream.write("The streaming API works!")
                outStream.endEncapsulation()

                //
                // Invoke operation.
                //
                let r = try obj.ice_invoke(operation: "printString", mode: .Normal, inEncaps: outStream.finished())
                if !r.ok {
                    print("Unknown user exception")
                }
            case .printStringSeq:
                //
                // Marshal the in parameter.
                //
                let outStream = Ice.OutputStream(communicator: communicator)
                outStream.startEncapsulation()
                outStream.write(["The", "streaming", "API", "works!"])
                outStream.endEncapsulation()

                //
                // Invoke operation.
                //
                let r = try obj.ice_invoke(operation: "printStringSequence",
                                           mode: .Normal,
                                           inEncaps: outStream.finished())
                if !r.ok {
                    print("Unknown user exception")
                }
            case .printDictionary:
                //
                // Marshal the in parameter.
                //
                let outStream = Ice.OutputStream(communicator: communicator)
                outStream.startEncapsulation()
                StringDictHelper.write(to: outStream, value: ["The": "streaming",
                                                              "API": "works!"])
                outStream.endEncapsulation()

                //
                // Invoke operation.
                //
                let r = try obj.ice_invoke(operation: "printDictionary", mode: .Normal, inEncaps: outStream.finished())
                if !r.ok {
                    print("Unknown user exception")
                }
            case .printEnum:
                //
                // Marshal the in parameter.
                //
                let outStream = Ice.OutputStream(communicator: communicator)
                outStream.startEncapsulation()
                outStream.write(.green)
                outStream.endEncapsulation()

                //
                // Invoke operation.
                //
                let r = try obj.ice_invoke(operation: "printEnum", mode: .Normal, inEncaps: outStream.finished())
                if !r.ok {
                    print("Unknown user exception")
                }
            case .printStruct:
                //
                // Marshal the in parameter.
                //
                let outStream = Ice.OutputStream(communicator: communicator)
                outStream.startEncapsulation()
                outStream.write(Structure(name: "red", value: Color.red))
                outStream.endEncapsulation()

                //
                // Invoke operation.
                //
                let r = try obj.ice_invoke(operation: "printStruct", mode: .Normal, inEncaps: outStream.finished())
                if !r.ok {
                    print("Unknown user exception")
                }
            case .printStructSequence:
                //
                // Marshal the in parameter.
                //
                let outStream = Ice.OutputStream(communicator: communicator)
                outStream.startEncapsulation()
                StructureSeqHelper.write(to: outStream,
                                         value: [Structure(name: "red", value: .red),
                                                 Structure(name: "green", value: .green),
                                                 Structure(name: "blue", value: .blue)])
                outStream.endEncapsulation()

                //
                // Invoke operation.
                //
                let r = try obj.ice_invoke(operation: "printStructSequence", mode: .Normal,
                                           inEncaps: outStream.finished())
                if !r.ok {
                    print("Unknown user exception")
                }
            case .printClass:
                //
                // Marshal the in parameter.
                //
                let outStream = Ice.OutputStream(communicator: communicator)
                outStream.startEncapsulation()
                outStream.write(C(s: Structure(name: "blue", value: .blue)))
                outStream.writePendingValues()
                outStream.endEncapsulation()

                //
                // Invoke operation.
                //
                let r = try obj.ice_invoke(operation: "printClass", mode: .Normal, inEncaps: outStream.finished())
                if !r.ok {
                    print("Unknown user exception")
                }
            case .getValues:
                //
                // Invoke operation.
                //
                let r = try obj.ice_invoke(operation: "getValues", mode: .Normal, inEncaps: Data())
                if !r.ok {
                    print("Unknown user exception")
                    continue
                }

                //
                // Unmarshal the results.
                //
                let inStream = Ice.InputStream(communicator: communicator, bytes: r.outEncaps)
                try inStream.startEncapsulation()
                var c: C?
                try inStream.read { c = $0 as? C }
                let str: String = try inStream.read()
                try inStream.readPendingValues()

                print("Got string `\(str)' and class: \(c!)")

            case .throwException:
                //
                // Invoke operation.
                //
                let r = try obj.ice_invoke(operation: "throwPrintFailure", mode: .Normal, inEncaps: Data())
                if r.ok {
                    print("Expected exception")
                    continue
                }

                let inStream = Ice.InputStream(communicator: communicator, bytes: r.outEncaps)
                try inStream.startEncapsulation()
                do {
                    try inStream.throwException()
                } catch is PrintFailure {
                    // Expected.
                } catch is Ice.UserException {
                    print("Unknown user exception")
                }
                try inStream.endEncapsulation()
            case .shutdownServer:
                //
                // Invoke operation.
                //
                let r = try obj.ice_invoke(operation: "shutdown", mode: .Normal, inEncaps: Data())
                if !r.ok {
                    print("Unknown user exception")
                }
            case .exit:
                exit = true
            case .help:
                menu()
            }
        } while !exit
        return 0
    } catch {
        print("Error: \(error)")
        return 1
    }
}

exit(run())
