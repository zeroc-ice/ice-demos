//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import InputBarAccessoryView
import MessageKit
import UIKit

extension UIColor {
    static let primaryColor = UIColor(red: 0.27, green: 0.50, blue: 0.82, alpha: 1.0)
}

class ChatLayoutController: MessagesViewController {

    var messages: [ChatMessage] = []
    var users: [ChatUser] = []
    var currentUser: ChatUser!

    let formatter = DateFormatter()

    override func viewDidLoad() {
        super.viewDidLoad()
        messagesCollectionView.messagesDataSource = self
        messagesCollectionView.messageCellDelegate = self
        messagesCollectionView.messagesLayoutDelegate = self
        messagesCollectionView.messagesDisplayDelegate = self
        formatter.dateStyle = .medium
    }

    func isLastSectionVisible() -> Bool {
        guard !messages.isEmpty else { return false }
        let lastIndexPath = IndexPath(item: 0, section: messages.count - 1)
        return messagesCollectionView.indexPathsForVisibleItems.contains(lastIndexPath)
    }
}

extension ChatLayoutController: MessagesDataSource {
    func currentSender() -> SenderType {
        return currentUser
    }

    func messageForItem(at indexPath: IndexPath, in _: MessagesCollectionView) -> MessageType {
        return messages[indexPath.section]
    }

    func numberOfSections(in _: MessagesCollectionView) -> Int {
        return messages.count
    }
}

extension ChatLayoutController: MessagesDisplayDelegate {
    func backgroundColor(for message: MessageType, at _: IndexPath, in _: MessagesCollectionView) -> UIColor {
        return isFromCurrentSender(message: message) ? .primaryColor : UIColor(red: 230 / 255,
                                                                               green: 230 / 255,
                                                                               blue: 230 / 255,
                                                                               alpha: 1)
    }

    func messageStyle(for message: MessageType, at _: IndexPath, in _: MessagesCollectionView) -> MessageStyle {
        let tail: MessageStyle.TailCorner = isFromCurrentSender(message: message) ? .bottomRight : .bottomLeft
        return .bubbleTail(tail, .curved)
    }

    func configureAvatarView(_ avatarView: AvatarView,
                             for message: MessageType,
                             at _: IndexPath,
                             in _: MessagesCollectionView) {
        let name = message.sender.displayName
        let avatar = Avatar(image: nil, initials: "\(name.first ?? "A")")
        avatarView.set(avatar: avatar)
    }
}

extension ChatLayoutController: MessageCellDelegate {
    func cellTopLabelAttributedText(for message: MessageType, at indexPath: IndexPath) -> NSAttributedString? {
        if indexPath.section % 7 == 0 {
            return NSAttributedString(string: MessageKitDateFormatter.shared.string(from: message.sentDate),
                                      attributes: [NSAttributedString.Key.font: UIFont.boldSystemFont(ofSize: 10),
                                                   NSAttributedString.Key.foregroundColor: UIColor.darkGray])
        }
        return nil
    }

    func messageTopLabelAttributedText(for message: MessageType, at _: IndexPath) -> NSAttributedString? {
        let name = message.sender.displayName
        let font = UIFont.preferredFont(forTextStyle: .caption1)
        return NSAttributedString(string: name,
                                  attributes: [NSAttributedString.Key.font: font])
    }

    func messageBottomLabelAttributedText(for message: MessageType, at _: IndexPath) -> NSAttributedString? {
        let dateString = formatter.string(from: message.sentDate)
        let font = UIFont.preferredFont(forTextStyle: .caption2)
        return NSAttributedString(string: dateString,
                                  attributes: [NSAttributedString.Key.font: font])
    }
}

extension ChatLayoutController: MessagesLayoutDelegate {
    func cellTopLabelHeight(for _: MessageType, at _: IndexPath, in _: MessagesCollectionView) -> CGFloat {
        return 18
    }

    func cellBottomLabelHeight(for _: MessageType, at _: IndexPath, in _: MessagesCollectionView) -> CGFloat {
        return 17
    }

    func messageTopLabelHeight(for _: MessageType, at _: IndexPath, in _: MessagesCollectionView) -> CGFloat {
        return 20
    }

    func messageBottomLabelHeight(for _: MessageType, at _: IndexPath, in _: MessagesCollectionView) -> CGFloat {
        return 16
    }
}
