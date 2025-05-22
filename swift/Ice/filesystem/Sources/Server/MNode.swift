// Copyright (c) ZeroC, Inc.

import Ice

/// Provides an in-memory implementation of the Slice interface Node.
class MNode: Node {
    private let name: String

    init(name: String) {
        self.name = name
    }

    // Implements Node.name.
    func name(current _: Ice.Current) -> String {
        return name
    }
}
