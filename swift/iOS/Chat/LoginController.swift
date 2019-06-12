//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Glacier2
import Ice
import PromiseKit
import UIKit

let usernameKey = "usernameKey"
let passwordKey = "passwordKey"
let sslKey = "sslKey"

class LoginController: UIViewController, UITextFieldDelegate, UIAlertViewDelegate {
    @IBOutlet var usernameField: UITextField!
    @IBOutlet var passwordField: UITextField!
    @IBOutlet var loginButton: UIButton!
    @IBOutlet var statusLabel: UILabel!
    @IBOutlet var statusActivity: UIActivityIndicatorView!

    weak var currentField: UITextField?
    var oldFieldValue: String?
    var chatController: ChatController?
    var communicator: Ice.Communicator?

    func initialize() {
        UserDefaults.standard.register(defaults: [usernameKey: "", passwordKey: "", sslKey: true])
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        // Set the default values, and show the clear button in the text field.
        usernameField.text = UserDefaults.standard.string(forKey: usernameKey)
        usernameField.clearButtonMode = .whileEditing
        passwordField.text = UserDefaults.standard.string(forKey: passwordKey)
        passwordField.clearButtonMode = .whileEditing

        let storyboard = UIStoryboard(name: "Main", bundle: nil)
        chatController = storyboard.instantiateViewController(withIdentifier: "ChatController") as? ChatController

        NotificationCenter.default.addObserver(self,
                                               selector: #selector(didEnterBackground),
                                               name: UIApplication.didEnterBackgroundNotification,
                                               object: nil)
    }

    func connecting(_ value: Bool) {
        // Show the wait alert.
        statusLabel.text = "Connecting..."
        statusLabel.isHidden = !value
        if value {
            statusActivity.startAnimating()
        } else {
            statusActivity.stopAnimating()
        }
        loginButton.isEnabled = !value
        usernameField.isEnabled = !value
        passwordField.isEnabled = !value
    }

    @objc func didEnterBackground() {
        if let communicator = communicator {
            DispatchQueue.global().async {
                communicator.destroy()
            }
        }
    }

    override func viewWillAppear(_ animated: Bool) {
        if (usernameField.text ?? "").isEmpty {
            loginButton.isEnabled = false
            loginButton.alpha = 0.5
        } else {
            loginButton.isEnabled = true
            loginButton.alpha = 1.0
        }
        super.viewWillAppear(animated) // No need for semicolon
    }

    func textFieldShouldBeginEditing(_ field: UITextField) -> Bool {
        currentField = field
        oldFieldValue = field.text
        return true
    }

    func textFieldShouldReturn(_ field: UITextField) -> Bool {
        precondition(field == currentField, "theTextField == currentTextField")

        // When the user presses return, take focus away from the text
        // field so that the keyboard is dismissed.

        if field == usernameField {
            UserDefaults.standard.set(field.text, forKey: usernameKey)
        } else if field == passwordField {
            UserDefaults.standard.set(field.text, forKey: passwordKey)
        }

        if (usernameField.text ?? "").isEmpty {
            loginButton.isEnabled = false
            loginButton.alpha = 0.5
        } else {
            loginButton.isEnabled = true
            loginButton.alpha = 1.0
        }

        field.resignFirstResponder()
        currentField = nil
        return true
    }

    //
    // A touch outside the keyboard dismisses the keyboard, and
    // sets back the old field value.
    //
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        if let field = currentField {
            field.resignFirstResponder()
            field.text = oldFieldValue
            currentField = nil
        }
        super.touchesBegan(touches, with: event)
    }

    func exception(_ message: String) {
        connecting(false)
        if (usernameField.text ?? "").isEmpty {
            loginButton.isEnabled = false
            loginButton.alpha = 0.5
        } else {
            loginButton.isEnabled = true
            loginButton.alpha = 1.0
        }

        //
        // open an alert with just an OK button
        //
        let alert = UIAlertController(title: "Error", message: message, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
        present(alert, animated: true, completion: nil)
    }

    @IBAction func login(_: Any) {
        let properties = Ice.createProperties()
        properties.setProperty(key: "Ice.Plugin.IceSSL", value: "1")
        properties.setProperty(key: "Ice.Default.Router",
                               value: "Glacier2/router:wss -p 443 -h zeroc.com -r /demo-proxy/chat/glacier2 -t 10000")

        properties.setProperty(key: "IceSSL.UsePlatformCAs", value: "1")
        properties.setProperty(key: "IceSSL.CheckCertName", value: "1")
        properties.setProperty(key: "Ice.ACM.Client.Timeout", value: "0")
        properties.setProperty(key: "Ice.RetryIntervals", value: "-1")

        var initData = Ice.InitializationData()
        initData.properties = properties

        connecting(true)

        let username: String = usernameField.text ?? ""
        let password: String = passwordField.text ?? ""

        var chatsession: ChatSessionPrx?
        var acmTimeout: Int32 = 0

        do {
            communicator = try Ice.initialize(initData)
            let router = uncheckedCast(prx: communicator!.getDefaultRouter()!, type: Glacier2.RouterPrx.self)

            router.createSessionAsync(userId: username, password: password).then { session -> Promise<Int32> in
                precondition(session != nil)
                chatsession = uncheckedCast(prx: session!, type: ChatSessionPrx.self)
                return router.getACMTimeoutAsync()
            }.then { timeout -> Promise<String> in
                acmTimeout = timeout
                return router.getCategoryForClientAsync()
            }.done { category -> Void in

                try self.chatController!.setup(communicator: self.communicator!,
                                               session: chatsession!,
                                               acmTimeout: acmTimeout,
                                               router: router,
                                               category: category)

                self.connecting(false)

                //
                // The communicator is now owned by the ChatController.
                //
                self.communicator = nil
                self.navigationController!.pushViewController(self.chatController!, animated: true)
            }.catch { err in
                if let ex = err as? Glacier2.CannotCreateSessionException {
                    self.exception("Session creation failed: \(ex.reason)")
                } else if let ex = err as? Glacier2.PermissionDeniedException {
                    self.exception("Login failed: \(ex.reason)")
                } else if let ex = err as? Ice.EndpointParseException {
                    self.exception("Invalid router: \(ex)")
                } else {
                    self.exception("Error: \(err)")
                }
            }
        } catch {
            exception("Error: \(error)")
        }
    }
}
