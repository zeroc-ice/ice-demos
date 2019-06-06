//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Glacier2
import Ice
import PromiseKit
import UIKit

struct Configuration {
    enum Keys: String {
        case user = "usernameKey"
        case password = "passwordKey"
        case ssl = "sslKey"
    }
}

class LoginController: UIViewController, UITextFieldDelegate, UIAlertViewDelegate {
    @IBOutlet var usernameField: UITextField!
    @IBOutlet var passwordField: UITextField!
    @IBOutlet var loginButton: LoadingButton!
    @IBOutlet var keyboardHeightLayoutConstraint: NSLayoutConstraint?

    weak var currentField: UITextField?
    var oldFieldValue: String?
    var communicator: Ice.Communicator?

    var connecting: Bool = false {
        didSet {
            loginButton.loading = connecting
            loginButton.isEnabled = !connecting
            usernameField.isEnabled = !connecting
            passwordField.isEnabled = !connecting
        }
    }

    func initialize() {
        UserDefaults.standard.register(defaults: [Configuration.Keys.user.rawValue: "",
                                                  Configuration.Keys.password.rawValue: "",
                                                  Configuration.Keys.ssl.rawValue: true])
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        // Set the default values, and show the clear button in the text field.
        usernameField.text = UserDefaults.standard.string(forKey: Configuration.Keys.user.rawValue)
        usernameField.clearButtonMode = .whileEditing
        passwordField.text = UserDefaults.standard.string(forKey: Configuration.Keys.password.rawValue)
        passwordField.clearButtonMode = .whileEditing

        NotificationCenter.default.addObserver(self,
                                               selector: #selector(keyboardWillShow(_:)),
                                               name: UIResponder.keyboardWillShowNotification,
                                               object: nil)
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(keyboardWillHide(_:)),
                                               name: UIResponder.keyboardWillHideNotification,
                                               object: nil)
    }

    deinit {
        NotificationCenter.default.removeObserver(self)
    }

    @objc func didEnterBackground() {
        if let communicator = communicator {
            DispatchQueue.global().async {
                communicator.destroy()
            }
        }
    }

    @objc func keyboardWillShow(_ notification: NSNotification) {
        let frame = notification.userInfo?[UIResponder.keyboardFrameBeginUserInfoKey] as? NSValue
        if let keyboardSize = frame?.cgRectValue {
            if view.frame.origin.y == 0 {
                view.frame.origin.y -= keyboardSize.height / 2
            }
        }
    }

    @objc func keyboardWillHide(_: NSNotification) {
        view.frame.origin.y = 0
    }

    override func viewWillAppear(_ animated: Bool) {
        if (usernameField.text ?? "").isEmpty {
            loginButton.isEnabled = false
            loginButton.alpha = 0.5
        } else {
            loginButton.isEnabled = true
            loginButton.alpha = 1.0
        }
        super.viewWillAppear(animated)
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
            UserDefaults.standard.set(field.text, forKey: Configuration.Keys.user.rawValue)
        } else if field == passwordField {
            UserDefaults.standard.set(field.text, forKey: Configuration.Keys.password.rawValue)
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
        connecting = false
        if (usernameField.text ?? "").isEmpty {
            loginButton.isEnabled = false
            loginButton.alpha = 0.5
        } else {
            loginButton.isEnabled = true
            loginButton.alpha = 1.0
        }

        let alert = UIAlertController(title: "Error", message: message, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
        present(alert, animated: true, completion: nil)
    }

    @IBAction func login(_: Any) {
        var properties: Properties {
            let prop = Ice.createProperties()
            prop.setProperty(key: "Ice.Plugin.IceSSL", value: "1")
            prop.setProperty(key: "Ice.Default.Router",
                             value: "Glacier2/router:wss -p 443 -h zeroc.com -r /demo-proxy/chat/glacier2 -t 10000")
            prop.setProperty(key: "IceSSL.UsePlatformCAs", value: "1")
            prop.setProperty(key: "IceSSL.CheckCertName", value: "1")
            prop.setProperty(key: "Ice.ACM.Client.Timeout", value: "0")
            prop.setProperty(key: "Ice.RetryIntervals", value: "-1")
            return prop
        }
        var initData = Ice.InitializationData()
        let chatController: ChatController?
        let storyboard = UIStoryboard(name: "Main", bundle: nil)
        chatController = storyboard.instantiateViewController(withIdentifier: "ChatController") as? ChatController
        initData.properties = properties

        connecting = true

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

                try chatController!.setup(communicator: self.communicator!,
                                               session: chatsession!,
                                               acmTimeout: acmTimeout,
                                               router: router,
                                               category: category)

                self.connecting = false
                chatController?.currentUser = ChatUser(name: username)

                //
                // The communicator is now owned by the ChatController.
                //
                self.communicator = nil
                self.navigationController!.pushViewController(chatController!, animated: true)
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
