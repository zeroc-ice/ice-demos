//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import SwiftUI
import MessageKit
import InputBarAccessoryView

final class MessageSwiftUIVC: MessagesViewController {
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        // Because SwiftUI wont automatically make our controller the first responder, we need to do it on viewDidAppear
        becomeFirstResponder()
        messagesCollectionView.scrollToLastItem(animated: true)
    }
}

struct MessagesView: UIViewControllerRepresentable {
    
    @State var initialized = false
    @Binding var messages: [ChatMessage]
    @Binding var currentUser: ChatUser
    
    func makeUIViewController(context: Context) -> MessagesViewController {
        let messagesVC = MessageSwiftUIVC()
        
        messagesVC.messagesCollectionView.messagesDisplayDelegate = context.coordinator
        messagesVC.messagesCollectionView.messagesLayoutDelegate = context.coordinator
        messagesVC.messagesCollectionView.messagesDataSource = context.coordinator
        messagesVC.messageInputBar.delegate = context.coordinator
        messagesVC.scrollsToLastItemOnKeyboardBeginsEditing = true // default false
        messagesVC.maintainPositionOnKeyboardFrameChanged = true // default false
        messagesVC.showMessageTimestampOnSwipeLeft = true // default false
        
        return messagesVC
    }
    
    func updateUIViewController(_ uiViewController: MessagesViewController, context: Context) {
        uiViewController.messagesCollectionView.reloadData()
        scrollToBottom(uiViewController)
    }
    
    private func scrollToBottom(_ uiViewController: MessagesViewController) {
        DispatchQueue.main.async {
            // The initialized state variable allows us to start at the bottom with the initial messages without seeing the initial scroll flash by
            uiViewController.messagesCollectionView.scrollToLastItem(animated: self.initialized)
            self.initialized = true
        }
    }
    
    func makeCoordinator() -> Coordinator {
        return Coordinator(messages: $messages, currentUser: $currentUser)
    }
    
    final class Coordinator {
        
        let formatter: DateFormatter = {
            let formatter = DateFormatter()
            formatter.dateStyle = .medium
            return formatter
        }()
        
        var messages: Binding<[ChatMessage]>
        var currentUser: Binding<ChatUser>
        init(messages: Binding<[ChatMessage]>, currentUser: Binding<ChatUser>) {
            self.messages = messages
            self.currentUser = currentUser
        }
        
    }
    
    private func isLastSectionVisible(_ uiViewController: MessagesViewController) -> Bool {
        guard !messages.isEmpty else { return false }
        let lastIndexPath = IndexPath(item: 0, section: messages.count - 1)
        return uiViewController.messagesCollectionView.indexPathsForVisibleItems.contains(lastIndexPath)
    }
}



extension MessagesView.Coordinator: MessagesDataSource {
    func currentSender() -> SenderType {
        return currentUser.wrappedValue
    }
    
    func messageForItem(at indexPath: IndexPath, in messagesCollectionView: MessagesCollectionView) -> MessageType {
        return messages.wrappedValue[indexPath.section]
    }
    
    func numberOfSections(in messagesCollectionView: MessagesCollectionView) -> Int {
        return messages.wrappedValue.count
    }
    
    func messageTopLabelAttributedText(for message: MessageType, at indexPath: IndexPath) -> NSAttributedString? {
        let name = message.sender.displayName
        let font = UIFont.preferredFont(forTextStyle: .caption1)
        return NSAttributedString(string: name,
                                  attributes: [NSAttributedString.Key.font: font])
    }
    
    func messageBottomLabelAttributedText(for message: MessageType, at indexPath: IndexPath) -> NSAttributedString? {
        let dateString = formatter.string(from: message.sentDate)
        let font = UIFont.preferredFont(forTextStyle: .caption2)
        return NSAttributedString(string: dateString,
                                  attributes: [NSAttributedString.Key.font: font])
    }

    func messageTimestampLabelAttributedText(for message: MessageType, at indexPath: IndexPath) -> NSAttributedString? {
        let sentDate = message.sentDate
        let sentDateString = MessageKitDateFormatter.shared.string(from: sentDate)
        let timeLabelFont: UIFont = .boldSystemFont(ofSize: 10)
        let timeLabelColor: UIColor = .systemGray
        return NSAttributedString(string: sentDateString, attributes: [NSAttributedString.Key.font: timeLabelFont, NSAttributedString.Key.foregroundColor: timeLabelColor])
    }
}

extension MessagesView.Coordinator: InputBarAccessoryViewDelegate {
    func inputBar(_ inputBar: InputBarAccessoryView, didPressSendButtonWith _: String) {
        let text = inputBar.inputTextView.text ?? ""
        inputBar.inputTextView.text = String()
        inputBar.invalidatePlugins()
        inputBar.sendButton.startAnimating()
        inputBar.inputTextView.placeholder = "Sending"
//        DispatchQueue.global(qos: .default).async {
//            self.session?.sendAsync(text).catch {
//                self.exception(err: $0)
//            }.finally {
//                self.messageInputBar.sendButton.stopAnimating()
//                self.messageInputBar.inputTextView.placeholder = "Message"
//                if self.isLastSectionVisible() == true {
//                    self.messagesCollectionView.scrollToBottom(animated: true)
//                }
//            }
//        }
    }
}

extension MessagesView.Coordinator: MessagesLayoutDelegate, MessagesDisplayDelegate {
    func configureAvatarView(_ avatarView: AvatarView, for message: MessageType, at indexPath: IndexPath, in messagesCollectionView: MessagesCollectionView) {
        let name = message.sender.displayName
        let avatar = Avatar(image: nil, initials: "\(name.first ?? "A")")
        avatarView.set(avatar: avatar)
    }
    
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
    
    func cellTopLabelHeight(for _: MessageType, at _: IndexPath, in _: MessagesCollectionView) -> CGFloat {
        return 18
    }

    func cellBottomLabelHeight(for _: MessageType, at _: IndexPath, in _: MessagesCollectionView) -> CGFloat {
        return 17
    }
    func messageTopLabelHeight(for message: MessageType, at indexPath: IndexPath, in messagesCollectionView: MessagesCollectionView) -> CGFloat {
        return 20
    }
    
    func messageBottomLabelHeight(for message: MessageType, at indexPath: IndexPath, in messagesCollectionView: MessagesCollectionView) -> CGFloat {
        return 16
    }
}
