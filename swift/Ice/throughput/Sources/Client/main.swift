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

        toggle type of data to send:
        1: sequence of bytes (default)
        2: sequence of strings (\"hello\")
        3: sequence of structs with a string (\"hello\") and a double
        4: sequence of structs with two ints and a double

        select test to run:
        t: Send sequence as twoway
        o: Send sequence as oneway
        r: Receive sequence
        e: Echo (send and receive) sequence

        other commands
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
        //
        // The communicator initialization removes all Ice-related arguments from args
        //
        guard args.count == 1 else {
            print("too many arguments\n")
            return 1
        }

        guard let obj = try communicator.propertyToProxy("Throughput.Proxy"),
            let throughput = try checkedCast(prx: obj, type: ThroughputPrx.self) else {
            print("invalid proxy")
            return 1
        }
        let throughputOneway = throughput.ice_oneway()

        let byteSeq = ByteSeq(repeating: 0, count: Int(ByteSeqSize))
        let stringSeq = StringSeq(repeating: "hello", count: Int(StringSeqSize))
        let structSeq = StringDoubleSeq(repeating: StringDouble(s: "hello", d: 3.14), count: Int(StringDoubleSeqSize))
        let fixedSeq = FixedSeq(repeating: Fixed(i: 0, j: 0, d: 0), count: Int(FixedSeqSize))

        menu()

        var line = ""
        var currentType = "1"

        repeat {
            print("==> ", terminator: "")
            guard let l = readLine(strippingNewline: true) else {
                return 0
            }
            line = l

            var seqSize = ByteSeqSize
            var repetitions = 100
            let start = DispatchTime.now()

            switch line {
            case "1", "2", "3", "4":
                currentType = line
                switch currentType {
                case "1":
                    print("using byte sequences")
                    seqSize = ByteSeqSize
                    repetitions = 1000 // Use more iterations for  byte sequences as it's a lot faster
                case "2":
                    print("using string sequences")
                    seqSize = StringSeqSize
                    repetitions = 100
                case "3":
                    print("using variable-length struct sequences")
                    seqSize = StringDoubleSeqSize
                    repetitions = 100
                case "4":
                    print("using fixed-length struct sequences")
                    seqSize = FixedSeqSize
                    repetitions = 100
                default:
                    precondition(false)
                }
            case "t", "o", "r", "e":
                switch line {
                case "t":
                    print("sending", terminator: "")
                case "o":
                    print("sending", terminator: "")
                case "r":
                    print("receiving", terminator: "")
                case "e":
                    print("sending and receiving", terminator: "")
                default:
                    precondition(false)
                }

                print(" \(repetitions) ", terminator: "")

                switch currentType {
                case "1":
                    print("byte", terminator: "")
                case "2":
                    print(" string", terminator: "")
                case "3":
                    print(" variable-length struct", terminator: "")
                case "4":
                    print(" fixed-length struct", terminator: "")
                default:
                    precondition(false)
                }
                print(" sequences of size \(seqSize)", terminator: "")

                if line == "o" {
                    print(" as oneway", terminator: "")
                }
                print("...")

                for _ in 0 ..< repetitions {
                    switch currentType {
                    case "1":
                        switch line {
                        case "t":
                            try throughput.sendByteSeq(byteSeq)
                        case "o":
                            try throughputOneway.sendByteSeq(byteSeq)
                        case "r":
                            _ = try throughput.recvByteSeq()
                        case "e":
                            _ = try throughput.echoByteSeq(byteSeq)
                        default:
                            precondition(false)
                        }

                    case "2":
                        switch line {
                        case "t":
                            try throughput.sendStringSeq(stringSeq)
                        case "o":
                            try throughputOneway.sendStringSeq(stringSeq)
                        case "r":
                            _ = try throughput.recvStringSeq()
                        case "e":
                            _ = try throughput.echoStringSeq(stringSeq)
                        default:
                            precondition(false)
                        }

                    case "3":
                        switch line {
                        case "t":
                            try throughput.sendStructSeq(structSeq)
                        case "o":
                            try throughputOneway.sendStructSeq(structSeq)
                        case "r":
                            _ = try throughput.recvStringSeq()
                        case "e":
                            _ = try throughput.echoStructSeq(structSeq)
                        default:
                            precondition(false)
                        }

                    case "4":
                        switch line {
                        case "t":
                            try throughput.sendFixedSeq(fixedSeq)
                        case "o":
                            try throughputOneway.sendFixedSeq(fixedSeq)
                        case "r":
                            _ = try throughput.recvFixedSeq()
                        case "e":
                            _ = try throughput.echoFixedSeq(fixedSeq)
                        default:
                            precondition(false)
                        }

                    default:
                        precondition(false)
                    }
                }

                let total = Double(DispatchTime.now().uptimeNanoseconds - start.uptimeNanoseconds) / 1_000_000

                var formated = String(format: "%.2f", total)
                print("time for \(repetitions) sequences: \(formated) ms")
                formated = String(format: "%.2f", total / Double(repetitions))
                print("time per sequence: \(formated) ms")

                var wireSize = 0
                switch currentType {
                case "1":
                    wireSize = 1
                case "2":
                    wireSize = stringSeq[0].count
                case "3":
                    wireSize = structSeq[0].s.count
                    wireSize += 8 // Size of double on the wire.
                case "4":
                    wireSize = 16 // Sizes of two ints and a double on the wire.
                default:
                    precondition(false)
                }

                var mbit = Double(Int32(repetitions) * Int32(seqSize) * Int32(wireSize)) * 8.0 / total / 1000.0
                if line == "e" {
                    mbit *= 2
                }
                formated = String(format: "%.2f", mbit)
                print("throughput: \(formated) Mbps")
            case "s":
                try throughput.shutdown()
            case "x":
                break
            case "?":
                menu()
            default:
                print("unknown command `\(line)'")
                menu()
            }
        } while line != "x"
        return 0
    } catch {
        print("Error \(error)\n")
        return 1
    }
}

exit(run())
