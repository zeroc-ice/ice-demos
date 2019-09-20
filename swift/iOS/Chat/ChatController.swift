//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import InputBarAccessoryView
import MessageKit
import PromiseKit
import UIKit

import Glacier2
import Ice

class ChatRoomCallbackInterceptor: Disp {
    private let servantDisp: Disp
    init(_ servantDisp: Disp) {
        self.servantDisp = servantDisp
    }

    func dispatch(request: Request, current: Current) throws -> Promise<Ice.OutputStream>? {
        //
        // Dispatch the request to the main thread in order
        // to modify the UI in a thread safe manner.
        //
        return try DispatchQueue.main.sync {
            try self.servantDisp.dispatch(request: request, current: current)
        }
    }
}

class ChatController: ChatLayoutController, ChatRoomCallback {
    var communicator: Ice.Communicator?
    var session: ChatSessionPrx?
    var router: Glacier2.RouterPrx?

    var callbackProxy: ChatRoomCallbackPrx?
    var acmTimeout: Int32 = 0

    func `init`(users: StringSeq, current _: Current) throws {
        self.users = users.map { ChatUser(name: $0) }
    }

    func send(timestamp: Int64, name: String, message: String, current _: Current) throws {
        append(ChatMessage(text: message, who: name, timestamp: timestamp))
    }

    func join(timestamp: Int64, name: String, current _: Current) throws {
        append(ChatMessage(text: "\(name) joined.", who: "System Message", timestamp: timestamp))
        users.append(ChatUser(name: name))
        navigationItem.rightBarButtonItem!.title = "\(users.count) " + (users.count > 1 ? "Users" : "User")
    }

    func leave(timestamp: Int64, name: String, current _: Current) throws {
        append(ChatMessage(text: "\(name) left.", who: "System Message", timestamp: timestamp))
        users.removeAll(where: { $0.displayName == name })
        navigationItem.rightBarButtonItem!.title = "\(users.count) " + (users.count > 1 ? "Users" : "User")
    }

    func append(_ message: ChatMessage) {
        if messages.count > 100 { // max 100 messages
            messages.remove(at: 0)
        }
        messages.append(message)
        messagesCollectionView.reloadData()
        messagesCollectionView.scrollToBottom(animated: true)
    }

    @objc func logout(_: UIBarButtonItem) {
        view.endEditing(true)
        destroySession()
        if let controller = navigationController {
            dismiss(animated: true, completion: nil)
            view.endEditing(true)
            controller.popViewController(animated: true)
        }
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        let button = UIBarButtonItem(title: "Logout", style: .done, target: self, action: #selector(logout(_:)))
        navigationItem.leftBarButtonItem = button

        messageInputBar.inputTextView.placeholder = "Message"
        messageInputBar.delegate = self
        messages = []
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(didEnterBackground),
                                               name: UIApplication.didEnterBackgroundNotification,
                                               object: nil)
        precondition(session != nil)
        precondition(router != nil)
        messagesCollectionView.reloadData()

        if let conn = router!.ice_getCachedConnection() {
            conn.setACM(timeout: acmTimeout, close: nil, heartbeat: .HeartbeatAlways)

            do {
                try conn.setCloseCallback { _ in
                    //
                    // Dispatch the connection close callback to the main thread
                    // in order to modify the UI in a thread safe manner.
                    //
                    DispatchQueue.main.async {
                        self.closed()
                    }
                }
            } catch {
            }
        }

        // Register the chat callback.
        firstly {
            session!.setCallbackAsync(callbackProxy)
        }.catch { err in
            self.exception(err: err)
        }
    }

    func setup(communicator: Ice.Communicator,
               session: ChatSessionPrx,
               acmTimeout: Int32,
               router: Glacier2.RouterPrx,
               category: String) throws {
        self.communicator = communicator
        self.session = session
        self.router = router
        self.acmTimeout = acmTimeout

        let adapter = try communicator.createObjectAdapterWithRouter(name: "ChatDemo.Client", rtr: router)
        try adapter.activate()
        let prx = try adapter.add(servant: ChatRoomCallbackInterceptor(ChatRoomCallbackDisp(self)),
                                  id: Ice.Identity(name: UUID().uuidString, category: category))
        callbackProxy = uncheckedCast(prx: prx, type: ChatRoomCallbackPrx.self)
    }

    override func prepare(for segue: UIStoryboardSegue, sender _: Any?) {
        if segue.identifier == "showUsers", let userController = segue.destination as? UserController {
            userController.users = users
        }
    }

    @objc func didEnterBackground() {
        destroySession()
        if let controller = navigationController {
            controller.popViewController(animated: false)
        }
    }

    func destroySession() {
        messages = []
        users = []
        currentUser = nil

        if let communicator = self.communicator {
            self.communicator = nil
            let router = self.router
            self.router = nil
            self.session = nil
            //
            // Destroy the session and the communicator asyncrhonously
            // to avoid blocking the main thread.
            //
            DispatchQueue.global().async {
                do {
                    try router?.destroySession()
                } catch {}
                communicator.destroy()
            }
        }
    }

    func exception(err: Error) {
        let alert = UIAlertController(title: "Error", message: "error: \(err)", preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
        present(alert, animated: true, completion: nil)

        destroySession()

        if let controller = navigationController {
            controller.popViewController(animated: true)
        }
    }

    func closed() {
        // The session is invalid, clear.
        if session != nil, let controller = navigationController {
            session = nil
            router = nil

            controller.popToRootViewController(animated: true)

            destroySession()

            // Open an alert with just an OK button
            let alert = UIAlertController(title: "Error",
                                          message: "Lost connection with session!\n",
                                          preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
            present(alert, animated: true, completion: nil)
        }
    }
}

extension ChatController: InputBarAccessoryViewDelegate {
    func inputBar(_ inputBar: InputBarAccessoryView, didPressSendButtonWith _: String) {
        let text = inputBar.inputTextView.text ?? ""
        messageInputBar.inputTextView.text = String()
        messageInputBar.invalidatePlugins()
        messageInputBar.sendButton.startAnimating()
        messageInputBar.inputTextView.placeholder = "Sending"
        DispatchQueue.global(qos: .default).async {
            self.session?.sendAsync(text).catch {
                self.exception(err: $0)
            }.finally {
                self.messageInputBar.sendButton.stopAnimating()
                self.messageInputBar.inputTextView.placeholder = "Message"
                if self.isLastSectionVisible() == true {
                    self.messagesCollectionView.scrollToBottom(animated: true)
                }
            }
        }
    }
}
