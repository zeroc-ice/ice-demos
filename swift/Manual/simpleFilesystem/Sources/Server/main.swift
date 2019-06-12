//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

/// Servant class for Node.
class NodeI: Node {
    private let name: String
    private let parent: DirectoryI?

    init(name: String, parent: DirectoryI?) {
        self.name = name
        self.parent = parent
    }

    // Slice Node::name() operation
    func name(current _: Ice.Current) -> String {
        return name
    }

    // Add servant to ASM and parent's contents map.
    func activate(adapter: Ice.ObjectAdapter) throws {
        let id = Ice.Identity(name: parent == nil ? "RootDir" : UUID().uuidString,
                              category: "")
        let prx = try adapter.add(servant: makeDisp(), id: id)

        // call addChild with proxy to `self` only if parent is not nil
        parent?.addChild(child: uncheckedCast(prx: prx, type: NodePrx.self))
    }

    // Create a dispatcher for servant `self`
    func makeDisp() -> Ice.Disp {
        fatalError("Abstract method")
    }
}

/// Servant class for File, reuses Node servant implementation.
class FileI: NodeI, File {
    private var lines: [String] = []

    // Slice File::read() operation
    func read(current _: Ice.Current) -> [String] {
        return lines
    }

    // Slice File::write() operation
    func write(text: [String], current _: Ice.Current) {
        lines = text
    }

    // Create a dispatcher for servant `self`
    override func makeDisp() -> Ice.Disp {
        return FileDisp(self)
    }
}

/// Servant class for Directory, reuses Node servant implementation.
class DirectoryI: NodeI, Directory {
    private var contents: [NodePrx?] = []

    // Slice Directory::list() operation
    func list(current _: Ice.Current) -> [NodePrx?] {
        return contents
    }

    // addChild is called by the child in order to add
    // itself to the contents member of the parent
    func addChild(child: NodePrx) {
        contents.append(child)
    }

    // Create a dispatcher for servant `self`
    override func makeDisp() -> Ice.Disp {
        return DirectoryDisp(self)
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
        file.write(text: ["This file system contains a collection of poetry."],
                   current: Ice.Current())
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
                          "Down to a sunless sea."],
                   current: Ice.Current())
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
