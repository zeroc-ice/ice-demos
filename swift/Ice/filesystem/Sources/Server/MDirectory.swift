// Copyright (c) ZeroC, Inc.

import Ice

/// Provides an in-memory implementation of the Slice interface Directory.
/// We use an actor since this servant has mutable state (contents).
actor MDirectory: Directory {
    private let node: MNode // reuse the MNode implementation
    private var contents: [NodePrx] = []

    init(name: String) {
        self.node = MNode(name: name)
    }

    // Implements Node.name by forwarding to node.
    func name(current: Ice.Current) -> String {
        node.name(current: current)
    }

    // Implements Directory.list.
    func list(current _: Ice.Current) -> [NodePrx?] {
        contents
    }

    // Adds a non-nil node proxy to this directory.
    func addChild(_ child: NodePrx) {
        contents.append(child)
    }
}
