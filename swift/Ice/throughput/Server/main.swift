//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice
import Foundation

class ThroughputI: Throughput {
    var byteSeq: ByteSeq
    var stringSeq: StringSeq
    var structSeq: StringDoubleSeq
    var fixedSeq: FixedSeq

    init() {
        byteSeq = ByteSeq(repeating: 0, count: Int(ByteSeqSize))
        stringSeq = StringSeq(repeating: "hello", count: Int(StringSeqSize))
        structSeq = StringDoubleSeq(repeating: StringDouble(s: "hello", d: 3.14),
                                    count: Int(StringDoubleSeqSize))
        fixedSeq = FixedSeq(repeating: Fixed(i: 0, j: 0, d: 0), count: Int(FixedSeqSize))
    }

    func needsWarmup(current: Current) throws -> Bool {
        return false
    }

    func startWarmup(current: Current) throws {}

    func endWarmup(current: Current) throws {}

    func sendByteSeq(seq: ByteSeq, current: Current) throws {}

    func recvByteSeq(current: Current) throws -> ByteSeq {
        return byteSeq
    }

    func echoByteSeq(seq: ByteSeq, current: Current) throws -> ByteSeq {
        return seq
    }

    func sendStringSeq(seq: StringSeq, current: Current) throws {}

    func recvStringSeq(current: Current) throws -> StringSeq {
        return stringSeq
    }

    func echoStringSeq(seq: StringSeq, current: Current) throws -> StringSeq {
        return seq
    }

    func sendStructSeq(seq: StringDoubleSeq, current: Current) throws {}

    func recvStructSeq(current: Current) throws -> StringDoubleSeq {
        return structSeq
    }

    func echoStructSeq(seq: StringDoubleSeq, current: Current) throws -> StringDoubleSeq {
        return seq
    }

    func sendFixedSeq(seq: FixedSeq, current: Current) throws {}

    func recvFixedSeq(current: Current) throws -> FixedSeq {
        return fixedSeq
    }

    func echoFixedSeq(seq: FixedSeq, current: Current) throws -> FixedSeq {
        return seq
    }

    func shutdown(current: Current) throws {
        if let adapter = current.adapter {
            adapter.getCommunicator().shutdown()
        }
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
        try adapter.add(servant: ThroughputI(), id: Ice.stringToIdentity("throughput"))
        try adapter.activate()

        communicator.waitForShutdown()
    } catch {
        print("Error: \(error)\n")
        return 1
    }
    return 0
}

exit(run())
