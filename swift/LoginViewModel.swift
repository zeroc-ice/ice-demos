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
import SwiftUI

//struct Configuration {
//    enum Keys: String {
//        case user = "usernameKey"
//        case password = "passwordKey"
//        case ssl = "sslKey"
//        case rememberMe = "rememberKey"
//    }
//}

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
}
