//
//  ChatMessage.swift
//  IOSChat
//
//  Created by Reece Humphreys on 6/18/19.
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
        kind = MessageKind.text(text)
        messageId = UUID().uuidString
    }
}
