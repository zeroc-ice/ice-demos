//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

extension Structure: CustomStringConvertible {
    public var description: String {
        return "name=\(name), value=\(value)"
    }
}

extension C: CustomStringConvertible {
    public var description: String {
        return "s.name=\(s.name), s.value=\(s.value)"
    }
}

class PrinterI: Ice.Blobject {
    func ice_invoke(inEncaps: Data, current: Current) throws -> (ok: Bool, outParams: Data) {
        let communicator = current.adapter!.getCommunicator()

        precondition(inEncaps.count >= 6, "Invalid incapsulation")
        let inStream = Ice.InputStream(communicator: communicator, bytes: inEncaps)
        try inStream.startEncapsulation()

        var ret: (ok: Bool, outParams: Data)
        switch current.operation {
        case "printString":
            let message: String = try inStream.read()
            print("Printing string `\(message)'")
            ret = (ok: true, outParams: Data())
        case "printStringSequence":
            let seq: StringSeq = try inStream.read()
            print("Printing string sequence \(seq)")
            ret = (ok: true, outParams: Data())
        case "printDictionary":
            let dict = try StringDictHelper.read(from: inStream)
            print("Printing dictionary \(dict)")
            ret = (ok: true, outParams: Data())
        case "printEnum":
            let c: Color = try inStream.read()
            print("Printing enum \(c)")
            ret = (ok: true, outParams: Data())
        case "printStruct":
            let s: Structure = try inStream.read()
            print("Printing struct: \(s)")
            ret = (ok: true, outParams: Data())
        case "printStructSequence":
            let seq = try StructureSeqHelper.read(from: inStream)
            print("Printing struct sequence: \(seq)")
            ret = (ok: true, outParams: Data())
        case "printClass":
            var c: C?
            try inStream.read { c = $0 as? C }
            try inStream.readPendingValues()
            try inStream.endEncapsulation()
            print("Printing class: \(c!)")
            return (ok: true, outParams: Data())
        case "getValues":
            let c = C(s: Structure(name: "green", value: .green))
            let outStream = Ice.OutputStream(communicator: communicator)
            outStream.startEncapsulation()
            outStream.write(c)
            outStream.write("hello")
            outStream.writePendingValues()
            outStream.endEncapsulation()
            ret = (ok: true, outParams: outStream.finished())
        case "throwPrintFailure":
            print("Throwing PrintFailure")
            let ex = PrintFailure(reason: "paper tray empty")
            let outStream = Ice.OutputStream(communicator: communicator)
            outStream.startEncapsulation()
            outStream.write(ex)
            outStream.endEncapsulation()
            ret = (ok: false, outParams: outStream.finished())
        case "shutdown":
            current.adapter!.getCommunicator().shutdown()
            ret = (ok: true, outParams: Data())
        default:
            throw Ice.OperationNotExistException(id: current.id, facet: current.facet, operation: current.operation)
        }

        try inStream.endEncapsulation()
        return ret
    }
}
