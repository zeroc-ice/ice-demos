//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice
import Foundation

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

        var inStream: Ice.InputStream!
        if inEncaps.count > 0 {
            inStream = Ice.InputStream(communicator: communicator, bytes: inEncaps)
            try inStream.startEncapsulation()
        }

        switch current.operation {
        case "printString":
            let message: String = try inStream.read()
            try inStream.endEncapsulation()
            print("Printing string `\(message)'")
            return (ok: true, outParams: Data())
        case "printStringSequence":
            let seq: StringSeq = try inStream.read()
            try inStream.endEncapsulation()
            print("Printing string sequence \(seq)")
            return (ok: true, outParams: Data())
        case "printDictionary":
            let dict = try StringDictHelper.read(from: inStream)
            try inStream.endEncapsulation()
            print("Printing dictionary \(dict)")
            return (ok: true, outParams: Data())
        case "printEnum":
            let c: Color = try inStream.read()
            try inStream.endEncapsulation()
            print("Printing enum \(c)")
            return (ok: true, outParams: Data())
        case "printStruct":
            let s: Structure = try inStream.read()
            try inStream.endEncapsulation()
            print("Printing struct: \(s)")
            return (ok: true, outParams: Data())
        case "printStructSequence":
            let seq = try StructureSeqHelper.read(from: inStream)
            try inStream.endEncapsulation()
            print("Printing struct sequence: \(seq)")
            return (ok: true, outParams: Data())
        case "printClass":
            var c: Value?
            try inStream.read { c = $0 }
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
            return (ok: true, outParams: outStream.finished())
        case "throwPrintFailure":
            print("Throwing PrintFailure")
            let ex = PrintFailure(reason: "paper tray empty")
            let outStream = Ice.OutputStream(communicator: communicator)
            outStream.startEncapsulation()
            outStream.write(ex)
            outStream.endEncapsulation()
            return (ok: false, outParams: outStream.finished())
        case "shutdown":
            current.adapter?.getCommunicator().shutdown()
            return (ok: true, outParams: Data())
        default:
            throw Ice.OperationNotExistException(id: current.id,
                                                 facet: current.facet,
                                                 operation: current.operation)
        }
    }
}
