//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

class FileI: File {
    var name: String
    var parent: DirectoryI
    var id: Ice.Identity
    var lines: [String]

    init(name: String, parent: DirectoryI) {
        self.name = name
        self.parent = parent

        //
        // Create an identity
        //
        id = Ice.Identity()
        id.name = UUID().uuidString

        self.lines = []
    }

    // Slice Node::name() operation
    func name(current _: Ice.Current) throws -> String {
        return name
    }

    // Slice File::read() operation
    func read(current _: Ice.Current) throws -> [String] {
        return lines
    }

    // Slice File::write() operation
    func write(text: [String], current _: Ice.Current = Ice.Current()) {
        lines = text
    }

    // Add servant to ASM and parent's _contents map.
    func activate(adapter: Ice.ObjectAdapter) throws {
        try parent.addChild(child: uncheckedCast(prx: adapter.add(servant: FileDisp(self), id: id),
                                                 type: NodePrx.self))
    }
}

class DirectoryI: Directory {
    var name: String
    var parent: DirectoryI?
    var id: Ice.Identity
    var contents: [NodePrx?]

    // DirectoryI constructor
    init(name: String, parent: DirectoryI?) {
        self.name = name
        self.parent = parent

        //
        // Create an identity. The root directory has the fixed identity "RootDir"
        //
        id = Ice.Identity()
        id.name = self.parent == nil ? "RootDir" : UUID().uuidString

        contents = []
    }

    // Slice Node::name() operation
    func name(current _: Ice.Current) throws -> String {
        return name
    }

    // Slice Directory::list() operation
    func list(current _: Ice.Current) throws -> [NodePrx?] {
        return contents
    }

    // addChild is called by the child in order to add
    // itself to the contents member of the parent
    func addChild(child: NodePrx) throws {
        contents.append(child)
    }

    // Add servant to ASM and parent's contents map.
    func activate(adapter: Ice.ObjectAdapter) throws {
        let thisNode = try uncheckedCast(prx: adapter.add(servant: DirectoryDisp(self), id: id),
                                         type: NodePrx.self)
        try parent?.addChild(child: thisNode)
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
        let root = DirectoryI(name: "/", parent: nil)
        try root.activate(adapter: adapter)

        //
        // Create a file called "README" in the root directory
        //
        var file = FileI(name: "README", parent: root)
        file.write(text: ["This file system contains a collection of poetry."])

        try file.activate(adapter: adapter)

        //
        // Create a directory called "Coleridge" in the root directory
        //
        let coleridge = DirectoryI(name: "Coleridge", parent: root)
        try coleridge.activate(adapter: adapter)

        //
        // Create a file called "Kubla_Khan" in the Coleridge directory
        //
        file = FileI(name: "Kubla_Khan", parent: coleridge)
        file.write(text: ["In Xanadu did Kubla Khan",
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
