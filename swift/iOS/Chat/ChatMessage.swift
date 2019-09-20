//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import MessageKit

class ChatMessage: MessageType {
    var sender: SenderType

    var messageId: String

    var sentDate: Date

    var kind: MessageKind

    init(text: String, who: String, timestamp: Int64) {
        let user = ChatUser(name: who)
        sender = user
        // Divide by 1000 to get correct date format
        sentDate = Date(timeIntervalSince1970: TimeInterval(integerLiteral: timestamp) / 1000)

        var data = text.replacingOccurrences(of: "&quot", with: "\"")
        data = data.replacingOccurrences(of: "&#39", with: "'")
        data = data.replacingOccurrences(of: "&lt;", with: "<")
        data = data.replacingOccurrences(of: "&gt;", with: ">")
        data = data.replacingOccurrences(of: "&amp;", with: "&")

        kind = MessageKind.text(data)
        messageId = UUID().uuidString
    }
}
