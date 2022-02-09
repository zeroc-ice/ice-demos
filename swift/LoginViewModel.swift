//
//  LoginViewModel.swift
//  IOSChat
//
//  Created by Reece Humphreys on 2/8/22.
//

import Glacier2
import Ice
import PromiseKit
import Combine

//struct Configuration {
//    enum Keys: String {
//        case user = "usernameKey"
//        case password = "passwordKey"
//        case ssl = "sslKey"
//        case rememberMe = "rememberKey"
//    }
//}

struct ChatConfig {
    var currentUser: ChatUser?
    var communicator: Ice.Communicator
    var session: ChatSessionPrx
    var acmTimeout: Int32
    var router: Glacier2.RouterPrx
    var category: String
}

class LoginViewModel: ObservableObject {
    
    
    
    @Published var username  = ""
    @Published var password  = ""
    @Published var rememberMe  = false {
        didSet { if rememberMe == false { initialize() } }
    }
    
    @Published var connecting: Bool = false {
        didSet {
//            loginButton.loading = connecting
//            loginButton.isEnabled = !connecting
//            usernameField.isEnabled = !connecting
//            passwordField.isEnabled = !connecting
        }
    }
    
    var communicator: Ice.Communicator?
    
    var isSigninComplete: Bool {
        if !isPasswordValid() || !isPasswordValid() {
            return false
        }
        return true
    }
    
    init(){
        configureDefaultValues()
    }
    
    // Validation methods
    private func isUsernameValid() -> Bool {
        return username != ""
    }
    
    private func isPasswordValid() -> Bool {
        return password != ""
    }
    
    // Storing and retrieving values from UserDefaults
    private func initialize(){
        UserDefaults.standard.register(defaults: [Configuration.Keys.user.rawValue: "",
                                                  Configuration.Keys.password.rawValue: "",
                                                  Configuration.Keys.ssl.rawValue: true,
                                                  Configuration.Keys.rememberMe.rawValue: false])
    }
    
    private func configureDefaultValues() {
        let bool = UserDefaults.standard.bool(forKey: Configuration.Keys.rememberMe.rawValue)
        rememberMe = bool
        if bool == true {
            username = UserDefaults.standard.string(forKey: Configuration.Keys.password.rawValue) ?? ""
            password = UserDefaults.standard.string(forKey: Configuration.Keys.user.rawValue) ?? ""
        }
    }
    
    public func storeDefaultValues(){
        UserDefaults.standard.set(username, forKey: Configuration.Keys.user.rawValue)
        UserDefaults.standard.set(password, forKey: Configuration.Keys.password.rawValue)
    }
    
    // Ice Login Logic
    
    
    public func attemptLogin(completionBlock: @escaping (Error?, ChatConfig?) -> Void){
        var properties: Properties {
            let prop = Ice.createProperties()
            prop.setProperty(key: "Ice.Plugin.IceSSL", value: "1")
            prop.setProperty(key: "Ice.Default.Router",
                             value: "Glacier2/router:wss -p 443 -h zeroc.com -r /demo-proxy/chat/glacier2 -t 10000")
            prop.setProperty(key: "IceSSL.UsePlatformCAs", value: "1")
            prop.setProperty(key: "IceSSL.CheckCertName", value: "1")
            prop.setProperty(key: "IceSSL.VerifyDepthMax", value: "5")
            prop.setProperty(key: "Ice.ACM.Client.Timeout", value: "0")
            prop.setProperty(key: "Ice.RetryIntervals", value: "-1")
            return prop
        }
        var initData = Ice.InitializationData()
        initData.properties = properties

        connecting = true


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
            }.done { [self] category in
                
                var chatConfig = ChatConfig(currentUser: ChatUser(name: username.lowercased()),
                                            communicator: communicator!,
                                            session: chatsession!,
                                            acmTimeout: acmTimeout,
                                            router: router,
                                            category: category)
                connecting = false

                //
                // The communicator is now owned by the ChatController.
                //
                if rememberMe == true {
                    UserDefaults.standard.set(username, forKey: Configuration.Keys.user.rawValue)
                    UserDefaults.standard.set(password, forKey: Configuration.Keys.password.rawValue)
                }
                print("Sucesss")
                communicator = nil
                completionBlock(nil, chatConfig)
            }.catch { err in
                print("Err")
                completionBlock(err, nil)
//                if let ex = err as? Glacier2.CannotCreateSessionException {
//                    self.exception("Session creation failed: \(ex.reason)")
//                } else if let ex = err as? Glacier2.PermissionDeniedException {
//                    self.exception("Login failed: \(ex.reason)")
//                } else if let ex = err as? Ice.EndpointParseException {
//                    self.exception("Invalid router: \(ex)")
//                } else {
//                    self.exception("Error: \(err)")
//                }
            }
        } catch {
            completionBlock(error, nil)
//            exception("Error: \(error)")
        }
    }
}
