// Copyright (c) ZeroC, Inc.

import Ice

/// Provides an in-memory implementation of the Slice interface Directory.
// This class reuses the implementation of Node provided by MNode.
final class MDirectory: MNode, Directory {
    private var contents: [NodePrx] = []

    // Implements Directory.list.
    func list(current _: Ice.Current) -> [NodePrx?] {
        return contents
    }

    // Adds a non-nill node proxy to this directory.
    func addChild(_ child: NodePrx) {
        contents.append(child)
    }
}
