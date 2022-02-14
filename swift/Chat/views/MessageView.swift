//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import InputBarAccessoryView
import MessageKit
import PromiseKit
import SwiftUI

final class MessageSwiftUIVC: MessagesViewController {
    var client: Client!

    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        // Because SwiftUI wont automatically make our controller the first responder, we need to do it on viewDidAppear
        becomeFirstResponder()
        messagesCollectionView.scrollToLastItem(animated: true)
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)

        let logoutButton = UIBarButtonItem(title: "Logout", style: .done, target: self, action: #selector(logout(_:)))
        parent?.navigationItem.leftBarButtonItem = logoutButton
        parent?.navigationItem.setHidesBackButton(true, animated: true)

        let usersText = "\(client.users.count) " + (client.users.count > 1 ? "Users" : "User")
        let usersButton = UIBarButtonItem(title: usersText,
                                          style: .plain,
                                          target: self,
                                          action: #selector(showUsers(_:)))
        parent?.navigationItem.rightBarButtonItem = usersButton
    }

    @objc public func showUsers(_: UIBarButtonItem) {
        let swiftUIView = UsersView(users: client?.users ?? [])
        let hostingController = UIHostingController(rootView: swiftUIView)
        present(hostingController, animated: true, completion: nil)
    }

    @objc func logout(_: UIBarButtonItem) {
        view.endEditing(true)
        client?.destroySession()
        if let controller = navigationController {
            dismiss(animated: true, completion: nil)
            view.endEditing(true)
            controller.popViewController(animated: true)
        }
    }
}

struct MessagesView: UIViewControllerRepresentable {
    @State var initialized = false
    @EnvironmentObject var client: Client

    func makeUIViewController(context: Context) -> MessageSwiftUIVC {
        let messagesVC = MessageSwiftUIVC()
        messagesVC.client = client
        messagesVC.messagesCollectionView.messagesDisplayDelegate = context.coordinator
        messagesVC.messagesCollectionView.messagesLayoutDelegate = context.coordinator
        messagesVC.messagesCollectionView.messagesDataSource = context.coordinator
        messagesVC.messageInputBar.delegate = context.coordinator
        messagesVC.scrollsToLastItemOnKeyboardBeginsEditing = true // default false
        messagesVC.maintainPositionOnKeyboardFrameChanged = true // default false
        messagesVC.showMessageTimestampOnSwipeLeft = true // default false
        messagesVC.messageInputBar.inputTextView.placeholder = "Message"

        client.messages = []

        precondition(client.session != nil)
        precondition(client.router != nil)

        if let conn = client.router!.ice_getCachedConnection() {
            conn.setACM(timeout: client.acmTimeout, close: nil, heartbeat: .HeartbeatAlways)

            do {
                try conn.setCloseCallback { _ in
                    //
                    // Dispatch the connection close callback to the main thread
                    // in order to modify the UI in a thread safe manner.
                    //
                    DispatchQueue.main.async {
                        self.closed(messagesVC)
                    }
                }
            } catch {}
        }

        // Register the chat callback.
        firstly {
            client.session!.setCallbackAsync(client.callbackProxy)
        }.catch { err in
            let alert = UIAlertController(title: "Error",
                                          message: err.localizedDescription,
                                          preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
            messagesVC.present(alert, animated: true, completion: nil)
        }
        return messagesVC
    }

    func updateUIViewController(_ uiViewController: MessageSwiftUIVC, context _: Context) {
        let usersText = "\(client.users.count) " + (client.users.count > 1 ? "Users" : "User")
        let usersButton = UIBarButtonItem(title: usersText,
                                          style: .plain,
                                          target: self,
                                          action: #selector(uiViewController.showUsers(_:)))
        uiViewController.parent?.navigationItem.rightBarButtonItem = usersButton
        uiViewController.messagesCollectionView.reloadData()
        if isLastSectionVisible(uiViewController) == false {
            scrollToBottom(uiViewController)
        }
    }

    func closed(_ uiViewController: MessageSwiftUIVC) {
        // The session is invalid, clear.
        if client.session != nil, let controller = uiViewController.navigationController {
            client.session = nil
            client.router = nil

            controller.popToRootViewController(animated: true)

            client.destroySession()

            // Open an alert with just an OK button
            let alert = UIAlertController(title: "Error",
                                          message: "Lost connection with session!\n",
                                          preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
            uiViewController.present(alert, animated: true, completion: nil)
        }
    }

    func makeCoordinator() -> Coordinator {
        return Coordinator(self, client: client)
    }

    final class Coordinator {
        let formatter: DateFormatter = {
            let formatter = DateFormatter()
            formatter.dateStyle = .medium
            return formatter
        }()

        var parent: MessagesView
        var client: Client
        init(_ parent: MessagesView, client: Client) {
            self.client = client
            self.parent = parent
        }
    }

    private func scrollToBottom(_ uiViewController: MessageSwiftUIVC) {
        DispatchQueue.main.async {
            // The initialized state variable allows us to start at the bottom with the initial messages without seeing
            // the initial scroll flash by
            uiViewController.messagesCollectionView.scrollToLastItem(animated: self.initialized)
            self.initialized = true
        }
    }

    private func isLastSectionVisible(_ uiViewController: MessagesViewController) -> Bool {
        guard !$client.messages.isEmpty else { return false }
        let lastIndexPath = IndexPath(item: 0, section: $client.messages.count - 1)
        return uiViewController.messagesCollectionView.indexPathsForVisibleItems.contains(lastIndexPath)
    }
}

extension MessagesView.Coordinator: MessagesDataSource {
    func currentSender() -> SenderType {
        return client.currentUser
    }

    func messageForItem(at indexPath: IndexPath, in _: MessagesCollectionView) -> MessageType {
        return client.messages[indexPath.section]
    }

    func numberOfSections(in _: MessagesCollectionView) -> Int {
        return client.messages.count
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

    func messageTimestampLabelAttributedText(for message: MessageType, at _: IndexPath) -> NSAttributedString? {
        let sentDate = message.sentDate
        let sentDateString = MessageKitDateFormatter.shared.string(from: sentDate)
        let timeLabelFont: UIFont = .boldSystemFont(ofSize: 10)
        let timeLabelColor: UIColor = .systemGray
        return NSAttributedString(string: sentDateString,
                                  attributes: [NSAttributedString.Key.font: timeLabelFont,
                                               NSAttributedString.Key.foregroundColor: timeLabelColor])
    }
}

extension MessagesView.Coordinator: InputBarAccessoryViewDelegate {
    func inputBar(_ inputBar: InputBarAccessoryView, didPressSendButtonWith _: String) {
        let text = inputBar.inputTextView.text ?? ""
        inputBar.inputTextView.text = String()
        inputBar.invalidatePlugins()
        inputBar.sendButton.startAnimating()
        inputBar.inputTextView.placeholder = "Sending"
        DispatchQueue.global(qos: .default).async {
            self.client.session?.sendAsync(text).catch { error in
                let viewController = inputBar.superview?.window?.rootViewController
                let alert = UIAlertController(title: "Error",
                                              message: error.localizedDescription,
                                              preferredStyle: .alert)
                alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
                viewController?.present(alert, animated: true, completion: nil)
            }.finally {
                inputBar.sendButton.stopAnimating()
                inputBar.inputTextView.placeholder = "Message"
            }
        }
    }
}

extension MessagesView.Coordinator: MessagesLayoutDelegate, MessagesDisplayDelegate {
    func configureAvatarView(_ avatarView: AvatarView,
                             for message: MessageType,
                             at _: IndexPath,
                             in _: MessagesCollectionView) {
        let name = message.sender.displayName
        let avatar = Avatar(image: nil, initials: "\(name.first ?? "A")")
        avatarView.set(avatar: avatar)
    }

    func backgroundColor(for message: MessageType, at _: IndexPath, in view: MessagesCollectionView) -> UIColor {
        let lightMode = view.traitCollection.userInterfaceStyle == .light
        let secondaryColor: UIColor = lightMode ? .secondaryLight : .secondaryDark
        return isFromCurrentSender(message: message) ? .primaryColor : secondaryColor
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

    func messageTopLabelHeight(for _: MessageType, at _: IndexPath, in _: MessagesCollectionView) -> CGFloat {
        return 20
    }

    func messageBottomLabelHeight(for _: MessageType, at _: IndexPath, in _: MessagesCollectionView) -> CGFloat {
        return 16
    }
}

extension UIColor {
    static let primaryColor = UIColor(red: 0.27, green: 0.50, blue: 0.82, alpha: 1.0)

    static let secondaryLight = UIColor(red: 230 / 255,
                                        green: 230 / 255,
                                        blue: 230 / 255,
                                        alpha: 1)
    static let secondaryDark = UIColor(red: 38 / 255,
                                       green: 37 / 255,
                                       blue: 41 / 255,
                                       alpha: 1)
}
