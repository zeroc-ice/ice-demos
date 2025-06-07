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

    // Since we define a `dispatch` method in MNode, we need to override it here. Otherwise, we'd use the base class
    // `dispatch`, not the one provided by generated Directory protocol extension.
    override func dispatch(_ request: sending Ice.IncomingRequest) async throws -> Ice.OutgoingResponse {
        // We implement dispatch by reusing the implementation provided by the generated Directory protocol extension.
        try await Self.dispatch(self, request: request)
    }
}
