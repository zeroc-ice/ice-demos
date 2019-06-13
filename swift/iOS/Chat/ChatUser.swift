//
//  ChatUser.swift
//  IOSChat
//
//  Created by Reece Humphreys on 6/18/19.
//

import MessageKit

public struct ChatUser: SenderType {
    public var senderId: String
    public let displayName: String

    init(name: String) {
        displayName = name
        senderId = name
    }
}
