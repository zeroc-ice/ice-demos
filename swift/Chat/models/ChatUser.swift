// Copyright (c) ZeroC, Inc.

import MessageKit

public struct ChatUser: SenderType, Identifiable {
    public var senderId: String
    public let displayName: String

    public var id: String {
        senderId
    }

    init(name: String) {
        displayName = name
        senderId = name.replacingOccurrences(of: " ", with: "").lowercased()
    }
}
