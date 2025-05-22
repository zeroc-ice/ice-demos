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
}
