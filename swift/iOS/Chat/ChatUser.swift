//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import MessageKit

public struct ChatUser: SenderType {
    public var senderId: String
    public let displayName: String

    init(name: String) {
        displayName = name
        senderId = name.replacingOccurrences(of: " ", with: "").lowercased()
    }
}
