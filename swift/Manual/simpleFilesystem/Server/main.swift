//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice
import Foundation

class FileI: File {

    var _name: String
    var _parent: DirectoryI
    var _id: Ice.Identity
    var _lines: [String]

    init(communicator: Ice.Communicator, name: String, parent: DirectoryI, lines: [String] = []) {
        _name = name
        _parent = parent

        //
        // Create an identity
        //
        _id = Ice.Identity()
        _id.name = UUID().uuidString

        _lines = lines
    }

    // Slice Node::name() operation
    func name(current: Ice.Current) throws -> String {
        return _name
    }

    // Slice File::read() operation
    func read(current: Ice.Current) throws -> [String] {
        return _lines
    }

    // Slice File::write() operation
    func write(text: [String], current: Ice.Current) {
        _lines = text
    }

    // Add servant to ASM and parent's _contents map.
    func activate(adapter: Ice.ObjectAdapter) throws {
        try _parent.addChild(child: uncheckedCast(prx: adapter.add(servant: self, id: _id), type: NodePrx.self))
    }
}

class DirectoryI: Directory {
    var _name: String
    var _parent: DirectoryI?
    var _id: Ice.Identity
    var _contents: [NodePrx?]
    // DirectoryI constructor

    init(communicator: Ice.Communicator, name: String, parent: DirectoryI?) {
        _name = name
        _parent = parent

        //
        // Create an identity. The root directory has the fixed identity "RootDir"
        //
        _id = Ice.Identity()
        _id.name = _parent == nil ? "RootDir" : UUID().uuidString

        _contents = []
    }

    // Slice Node::name() operation
    func name(current: Ice.Current) throws -> String {
        return _name
    }

    // Slice Directory::list() operation
    func list(current: Ice.Current) throws -> [NodePrx?] {
        return _contents
    }

    // addChild is called by the child in order to add
    // itself to the _contents member of the parent
    func addChild(child: NodePrx) throws {
        _contents.append(child)
    }

    // Add servant to ASM and parent's _contents map.
    func activate(adapter: Ice.ObjectAdapter) throws {
        let thisNode = try uncheckedCast(prx: adapter.add(servant: self, id: _id), type: NodePrx.self)
        if let parent = _parent {
            try parent.addChild(child: thisNode)
        }
    }
}

func run() -> Int32 {
    do {
        let communicator = try Ice.initialize(CommandLine.arguments)
        defer {
            communicator.destroy()
        }

        //
        // Create an object adapter.
        //
        let adapter = try communicator.createObjectAdapterWithEndpoints(name: "SimpleFilesystem",
                                                                        endpoints: "default -h localhost -p 10000")

        //
        // Create the root directory (with name "/" and no parent)
        //
        let root = DirectoryI(communicator: communicator, name: "/", parent: nil)
        try root.activate(adapter: adapter)

        //
        // Create a file called "README" in the root directory
        //
        var file = FileI(communicator: communicator, name: "README", parent: root,
                         lines: ["This file system contains a collection of poetry."])
        try file.activate(adapter: adapter)

        //
        // Create a directory called "Coleridge" in the root directory
        //
        let coleridge = DirectoryI(communicator: communicator, name: "Coleridge", parent: root)
        try coleridge.activate(adapter: adapter)

        //
        // Create a file called "Kubla_Khan" in the Coleridge directory
        //
        file = FileI(communicator: communicator, name: "Kubla_Khan", parent: coleridge,
                     lines: ["In Xanadu did Kubla Khan",
                             "A stately pleasure-dome decree:",
                             "Where Alph, the sacred river, ran",
                             "Through caverns measureless to man",
                             "Down to a sunless sea."])
        try file.activate(adapter: adapter)

        //
        // All objects are created, allow client requests now
        //
        try adapter.activate()

        //
        // Wait until we are done
        //
        communicator.waitForShutdown()
        return 0
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
