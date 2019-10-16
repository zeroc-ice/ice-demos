//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

// Automatically flush stdout
setbuf(__stdoutp, nil)

class ThroughputI: Throughput {
    var byteSeq: ByteSeq
    var stringSeq: StringSeq
    var structSeq: StringDoubleSeq
    var fixedSeq: FixedSeq

    init() {
        byteSeq = ByteSeq(repeating: 0, count: Int(ByteSeqSize))
        stringSeq = StringSeq(repeating: "hello", count: Int(StringSeqSize))
        structSeq = StringDoubleSeq(repeating: StringDouble(s: "hello", d: 3.14), count: Int(StringDoubleSeqSize))
        fixedSeq = FixedSeq(repeating: Fixed(i: 0, j: 0, d: 0), count: Int(FixedSeqSize))
    }

    func needsWarmup(current _: Current) throws -> Bool {
        return false
    }

    func startWarmup(current _: Current) throws {}

    func endWarmup(current _: Current) throws {}

    func sendByteSeq(seq: ByteSeq, current _: Current) throws {}

    func recvByteSeq(current _: Current) throws -> ByteSeq {
        return byteSeq
    }

    func echoByteSeq(seq: ByteSeq, current _: Current) throws -> ByteSeq {
        return seq
    }

    func sendStringSeq(seq: StringSeq, current _: Current) throws {}

    func recvStringSeq(current _: Current) throws -> StringSeq {
        return stringSeq
    }

    func echoStringSeq(seq: StringSeq, current _: Current) throws -> StringSeq {
        return seq
    }

    func sendStructSeq(seq _: StringDoubleSeq, current _: Current) throws {}

    func recvStructSeq(current _: Current) throws -> StringDoubleSeq {
        return structSeq
    }

    func echoStructSeq(seq: StringDoubleSeq, current _: Current) throws -> StringDoubleSeq {
        return seq
    }

    func sendFixedSeq(seq _: FixedSeq, current _: Current) throws {}

    func recvFixedSeq(current _: Current) throws -> FixedSeq {
        return fixedSeq
    }

    func echoFixedSeq(seq: FixedSeq, current _: Current) throws -> FixedSeq {
        return seq
    }

    func shutdown(current: Current) throws {
        current.adapter!.getCommunicator().shutdown()
    }
}

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

        let adapter = try communicator.createObjectAdapter("Throughput")
        try adapter.add(servant: ThroughputDisp(ThroughputI()), id: Ice.stringToIdentity("throughput"))
        try adapter.activate()

        communicator.waitForShutdown()
        return 0
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
