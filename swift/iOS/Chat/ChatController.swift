//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import UIKit
import PromiseKit
import Foundation

import Ice
import Glacier2

struct ChatMessage {
    let timestamp: Date
    let who: String
    let text: String

    init(text: String, who: String, timestamp: Int64) {
        self.who = who
        let htmlEntities = ["&quot;": "\"",
                            "&#39;": "'",
                            "&lt;": "<",
                            "&gt;": ">",
                            "&amp;": "&"]

        var s = text
        for (key, value) in htmlEntities {
            s = s.replacingOccurrences(of: key, with: value)
        }
        self.text = s
        //
        // The ChatMessage timestamp is ms since the UNIX epoch.
        //
        self.timestamp = Date(timeIntervalSince1970: TimeInterval(timestamp / 1000))
    }
}

class MessageCell: UITableViewCell {
    @IBOutlet weak var timestamp: UILabel!
    @IBOutlet weak var who: UILabel!
    @IBOutlet weak var body: UILabel!

    var dateFormatter: DateFormatter
    var message: ChatMessage!

    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        dateFormatter = DateFormatter()
        dateFormatter.dateStyle = .short
        dateFormatter.timeStyle = .medium
        super.init(style: style, reuseIdentifier: reuseIdentifier)
    }

    required init?(coder: NSCoder) {
        dateFormatter = DateFormatter()
        dateFormatter.dateStyle = .short
        dateFormatter.timeStyle = .medium
        super.init(coder: coder)
    }

    func setMessage(_ message: ChatMessage) {
        self.message = message
        timestamp.text = dateFormatter.string(from: message.timestamp)
        who.text = message.who
        body.text = message.text
    }
}

class ChatController: UIViewController,
                      UITableViewDelegate,
                      UITableViewDataSource,
                      UITextFieldDelegate,
                      ChatRoomCallback {

    @IBOutlet weak var chatView: UITableView!
    @IBOutlet weak var inputField: UITextField!

    var users: [String] = []
    var messages: [ChatMessage] = []

    var communicator: Ice.Communicator?
    var session: ChatSessionPrx?
    var router: Glacier2.RouterPrx?

    var callbackProxy: ChatRoomCallbackPrx?
    var acmTimeout: Int32 = 0

    override func viewDidLoad() {
        super.viewDidLoad()

        messages = []

        NotificationCenter.default.addObserver(self,
                                               selector: #selector(didEnterBackground),
                                               name: UIApplication.didEnterBackgroundNotification,
                                               object: nil)

        precondition(session != nil)
        precondition(router != nil)

        chatView.reloadData()

        if let conn = router!.ice_getCachedConnection() {
            conn.setACM(timeout: acmTimeout, close: nil, heartbeat: .HeartbeatAlways)

            do {
                try conn.setCloseCallback { _ in
                    self.closed()
                }
            } catch {}
        }

        // Register the chat callback.
        firstly {
            session!.setCallbackAsync(callbackProxy)
        }.catch { error in
            self.exception(err: error)
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

        // The callback is registered in clear:, otherwise the callbacks can arrive
        // prior to the IBOutlet connections being setup.
        let prx = try adapter.add(servant: self, id: Ice.Identity(name: UUID().uuidString, category: category))
        self.callbackProxy = uncheckedCast(prx: prx, type: ChatRoomCallbackPrx.self)
    }

    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get reference to the destination view controller
        if segue.identifier == "showUsers", let userController = segue.destination as? UserController {
            // Pass any objects to the view controller here, like...
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
        //
        // Destroy the session and destroy the communicator from another thread since these
        // calls block.
        //
        if let communicator = self.communicator, let router = self.router {
            self.session = nil
            self.communicator = nil
            self.router = nil
            DispatchQueue.global().async {
                do {
                    try router.destroySession()
                } catch {}
                communicator.destroy()
            }
        }
    }

    func exception(err: Error) {
        //
        // Open an alert with just an OK button
        //
        let alert = UIAlertController(title: "Error", message: "error: \(err)", preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
        present(alert, animated: true, completion: nil)

        destroySession()

        if let controller = navigationController {
            controller.popViewController(animated: true)
        }
    }

    func closed() {
        //
        // The session is invalid, clear.
        //
        if self.session != nil, let controller = self.navigationController {
            controller.popToRootViewController(animated: true)

            //
            // The session is invalid, clear.
            //
            self.session = nil
            self.router = nil

            //
            // Clean up the remainder.
            //
            self.destroySession()

            //
            // Open an alert with just an OK button
            //
            let alert = UIAlertController(title: "Error",
                                          message: "Lost connection with session!\n",
                                          preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
            present(alert, animated: true, completion: nil)
        }
    }

    @IBAction func logout() {
        view.endEditing(true)
        destroySession()
        if let controller = navigationController {
            controller.popViewController(animated: true)
        }
    }

    func append(_ message: ChatMessage) {
        DispatchQueue.main.async {
            if self.messages.count > 100 { // max 100 messages
                self.messages.remove(at: 0)
            }

            self.messages.append(message)
            self.chatView.reloadData()
            /*self.chatView.scrollToRow(at: IndexPath(indexes: [0, self.messages.count - 1]),
                                      at: .bottom, animated: false)*/
        }
    }

    func `init`(users: StringSeq, current: Current) throws {
        self.users = users
    }

    func send(timestamp: Int64, name: String, message: String, current: Current) throws {
        append(ChatMessage(text: message, who: name, timestamp: timestamp))
    }

    func join(timestamp: Int64, name: String, current: Current) throws {
        append(ChatMessage(text: "\(name) joined.\n", who: "system message", timestamp: timestamp))
        users.append(name)
        navigationItem.rightBarButtonItem!.title = "\(users.count) " + (users.count > 1 ? "Users" : "User")
    }

    func leave(timestamp: Int64, name: String, current: Current) throws {
        append(ChatMessage(text: "\(name) left.\n", who: "system message", timestamp: timestamp))
        users.removeAll(where: { $0 == name })
        navigationItem.rightBarButtonItem!.title = "\(users.count) " + (users.count > 1 ? "Users" : "User")
    }

    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }

    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return messages.count
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = chatView.dequeueReusableCell(withIdentifier: "MessageCell") as! MessageCell
        cell.setMessage(messages[indexPath.row])
        return cell
    }

    override func viewWillAppear(_ animated: Bool) {
        //
        // Register for keyboard show/hide notifications.
        //
        NotificationCenter.default.addObserver(self, selector: #selector(keyboardWillShow),
                                               name: UIResponder.keyboardWillShowNotification, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(keyboardWillHide),
                                               name: UIResponder.keyboardWillHideNotification, object: nil)
    }

    override func viewWillDisappear(_ animated: Bool) {
        view.endEditing(true)

        //
        // Unregister for keyboard show/hide notifications.
        //
        NotificationCenter.default.removeObserver(self, name: UIResponder.keyboardWillShowNotification, object: nil)
        NotificationCenter.default.removeObserver(self, name: UIResponder.keyboardWillHideNotification, object: nil)
    }

    @objc func keyboardWillShow(_ notification: NSNotification) {
        if let keyboardSize = notification.userInfo?[UIResponder.keyboardFrameBeginUserInfoKey] as? CGRect {
            self.view.frame.origin.y -= keyboardSize.height
        }
    }

    @objc func keyboardWillHide(_ notification: NSNotification) {
        self.view.frame.origin.y = 0
    }

    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        inputField.resignFirstResponder()
        inputField.text = ""
        super.touchesBegan(touches, with: event)
    }

    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        if var s = textField.text?.trimmingCharacters(in: CharacterSet.whitespaces) {
            if s.count > 1024 {
                s = String(s.dropLast(s.count - 1024))
            }

            if !s.isEmpty {
                session?.sendAsync(s).catch {
                    self.exception(err: $0)
                }
            }

            textField.text = ""
            textField.resignFirstResponder()
        }
        return true
    }
}
