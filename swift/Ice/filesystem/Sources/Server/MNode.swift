// Copyright (c) ZeroC, Inc.

import Ice

/// Provides an in-memory implementation of the Slice interface Node.
// MNode conforms to the generated Node protocol, which inherits from the Ice.Dispatcher protocol.
class MNode: Node {
    private let name: String

    init(name: String) {
        self.name = name
    }

    // Implements Node.name.
    func name(current _: Ice.Current) -> String {
        return name
    }

    // Since we want to reuse this servant class (MNode) in derived servant classes, we need to define an overridable
    // `dispatch` method. The default `dispatch` method provided by the generated Node protocol extension is not
    // overridable. See https://github.com/swiftlang/swift/issues/42725.
    func dispatch(_ request: sending Ice.IncomingRequest) async throws -> Ice.OutgoingResponse {
        // We implement dispatch by reusing the implementation provided by the generated Node protocol extension.
        try await Self.dispatch(self, request: request)
    }
}
