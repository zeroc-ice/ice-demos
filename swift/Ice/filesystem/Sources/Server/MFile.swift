// Copyright (c) ZeroC, Inc.

import Ice

/// Provides an in-memory implementation of the Slice interface File.
/// We use an actor since this servant has mutable state (the file contents).
actor MFile: File {
    private let node: MNode // reuse the MNode implementation
    private var lines: [String] = []

    init(name: String) {
        self.node = MNode(name: name)
    }

    // Implements Node.name by forwarding to node.
    func name(current: Ice.Current) -> String {
        node.name(current: current)
    }

    // Implements File.read.
    func read(current _: Ice.Current) -> [String] {
        lines
    }

    // Implements File.write.
    func write(text: [String], current _: Ice.Current) {
        writeDirect(text: text)
    }

    // Writes directly to this file, without going through an Ice operation.
    func writeDirect(text: [String]) {
        lines = text
    }
}
