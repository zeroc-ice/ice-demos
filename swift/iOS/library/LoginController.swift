//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Glacier2
import Ice

import Foundation

import PromiseKit
import UIKit

let usernameKey = "usernameKey"
let passwordKey = "passwordKey"

protocol SessionAdapter {
    @discardableResult func destroy() -> Promise<Void>
}

class Glacier2SessionAdapter: SessionAdapter {
    let router: Glacier2.RouterPrx

    init(_ router: Glacier2.RouterPrx) {
        self.router = router
    }

    @discardableResult func destroy() -> Promise<Void> {
        return router.destroySessionAsync()
    }
}

class DemoSessionAdapter: SessionAdapter {
    let session: SessionPrx

    init(_ session: SessionPrx) {
        self.session = session
    }

    @discardableResult func destroy() -> Promise<Void> {
        return session.destroyAsync()
    }
}

class LoginController: UIViewController,
    UITextFieldDelegate,
    UIAlertViewDelegate {
    @IBOutlet var usernameField: UITextField!
    @IBOutlet var passwordField: UITextField!
    @IBOutlet var loginButton: UIButton!
    @IBOutlet var statusLabel: UILabel!
    @IBOutlet var statusActivity: UIActivityIndicatorView!

    var currentField: UITextField!
    var oldFieldValue: String?

    var mainController: MainController!

    var communicator: Ice.Communicator!
    var library: LibraryPrx?
    var initializationData: Ice.InitializationData!

    override func viewDidLoad() {
        super.viewDidLoad()
        do {
            // Load initialization data here so that we check if the username/passwords fields should be hidden.
            let properties = Ice.createProperties()
            let resourcesPath = Bundle.main.resourcePath!
            try properties.load("\(resourcesPath)/config.client")
            properties.setProperty(key: "Ice.Plugin.IceSSL", value: "1")
            properties.setProperty(key: "Ice.RetryIntervals", value: "-1")

            initializationData = Ice.InitializationData()
            initializationData.properties = properties

            //
            // If Ice.Default.Router is not set then we hide the login and password prompts.
            //
            if properties.getProperty("Ice.Default.Router").isEmpty {
                usernameField.superview!.isHidden = true
                passwordField.superview!.isHidden = true
                loginButton.isEnabled = true
            } else {
                //
                // Set the default values
                //
                usernameField.text = UserDefaults.standard.string(forKey: usernameKey)
                passwordField.text = UserDefaults.standard.string(forKey: passwordKey)
            }

            let mainStoryboard = UIStoryboard(name: "Main", bundle: nil)
            mainController =
                mainStoryboard.instantiateViewController(withIdentifier: "MainController") as? MainController

            NotificationCenter.default.addObserver(self,
                                                   selector: #selector(applicationWillTerminate),
                                                   name: UIApplication.willTerminateNotification,
                                                   object: nil)
        } catch {
            fatalError()
        }
    }

    @objc func applicationWillTerminate() {
        if let communicator = self.communicator {
            communicator.destroy()
        }
    }

    override func viewWillAppear(_ animated: Bool) {
        if usernameField.superview!.isHidden || !usernameField.text!.isEmpty {
            loginButton.isEnabled = true
            loginButton.alpha = 1.0
        } else {
            loginButton.isEnabled = false
            loginButton.alpha = 0.5
        }
        super.viewWillAppear(animated)
    }

    func textFieldDidBeginEditing(_ field: UITextField) {
        currentField = field
        oldFieldValue = field.text
    }

    func textFieldDidEndEditing(_ field: UITextField) {
        precondition(field == currentField)
        //
        // When the user presses return, take focus away from the text
        // field so that the keyboard is dismissed.
        //
        UserDefaults.standard.set(field.text, forKey: field == usernameField ? usernameKey : passwordKey)

        if usernameField.text!.isEmpty {
            loginButton.isEnabled = false
            loginButton.alpha = 0.5
        } else {
            loginButton.isEnabled = true
            loginButton.alpha = 1.0
        }

        field.resignFirstResponder()
        currentField = nil
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

        // We always create a new communicator each time
        // we try to login.
        if let communicator = self.communicator {
            communicator.destroy()
        }
        communicator = nil

        //
        // open an alert with just an OK button
        //
        let alert = UIAlertController(title: "Error", message: message, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
        present(alert, animated: true, completion: nil)
    }

    func connecting(_ connecting: Bool) {
        statusLabel.isHidden = !connecting
        if connecting {
            statusActivity.startAnimating()
        } else {
            statusActivity.stopAnimating()
        }
        usernameField.isEnabled = !connecting
        passwordField.isEnabled = !connecting
        loginButton.isEnabled = !connecting
    }

    @IBAction func login(sender _: Any?) {
        do {
            precondition(communicator == nil)
            communicator = try Ice.initialize(initializationData)

            if let router = communicator.getDefaultRouter() {
                connecting(true)

                let glacier2router = uncheckedCast(prx: router, type: Glacier2.RouterPrx.self)
                let username = usernameField.text!
                let password = passwordField.text!

                glacier2router.createSessionAsync(userId: username, password: password).then { session in
                    uncheckedCast(prx: session!, type: Glacier2SessionPrx.self).getLibraryAsync()
                }.then { library -> Promise<Int32> in
                    self.library = library
                    return glacier2router.getACMTimeoutAsync()
                }.done { acmTimeout in
                    if acmTimeout > 0 {
                        //
                        // Configure the connection to send heartbeats in order to keep our session alive
                        //
                        glacier2router.ice_getCachedConnection()!.setACM(timeout: acmTimeout,
                                                                         close: nil,
                                                                         heartbeat: .HeartbeatAlways)
                    }
                    self.connecting(false)
                    self.mainController.activate(communicator: self.communicator,
                                                 session: Glacier2SessionAdapter(glacier2router),
                                                 library: self.library)

                    self.library = nil
                    self.communicator = nil
                    self.navigationController!.pushViewController(self.mainController, animated: true)
                }.catch { error in
                    self.exception("Error: \(error)")
                }
            } else {
                let properties = communicator.getProperties()
                guard let proxy = try communicator.stringToProxy(properties.getProperty("SessionFactory.Proxy")) else {
                    exception("Error neither Ice.Default.Router nor SessionFactory.Proxy" +
                        "are defined in the application config file")
                    return
                }
                connecting(true)
                let factory = uncheckedCast(prx: proxy, type: SessionFactoryPrx.self)
                var session: SessionPrx!
                factory.createAsync().then { s -> Promise<LibraryPrx?> in
                    session = s
                    return session.getLibraryAsync()
                }.done { library in
                    self.connecting(false)
                    self.mainController.activate(communicator: self.communicator,
                                                 session: DemoSessionAdapter(session),
                                                 library: library)

                    self.library = nil
                    self.communicator = nil
                    self.navigationController!.pushViewController(self.mainController!, animated: true)
                }.catch { error in
                    self.exception("Error: \(error)")
                }
            }

        } catch {
            exception("Error: \(error)")
        }
    }
}
