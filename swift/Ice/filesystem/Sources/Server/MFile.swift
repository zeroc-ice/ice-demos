// Copyright (c) ZeroC, Inc.

import Ice

/// Provides an in-memory implementation of the Slice interface File.
// This class reuses the implementation of Node provided by MNode.
final class MFile: MNode, File {
    private var lines: [String] = []

    // Implements File.read.
    func read(current _: Ice.Current) -> [String] {
        return lines
    }

    // Implements File.write.
    func write(text: [String], current _: Ice.Current) {
        writeDirect(text: text)
    }

    // Writes directly to this file, without going through an Ice operation.
    func writeDirect(text: [String]) {
        lines = text
    }

    // Since we define a `dispatch` method in MNode, we need to override it here. Otherwise, we'd use the base class
    // `dispatch`, not the one provided by the generated File protocol extension.
    override func dispatch(_ request: Ice.IncomingRequest) async throws -> Ice.OutgoingResponse {
        // We implement dispatch by reusing the implementation provided by the generated File protocol extension.
        try await Self.dispatch(self, request: request)
    }
}
