//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Combine
import Glacier2
import Ice
import PromiseKit
import SwiftUI

struct Configuration {
    enum Keys: String {
        case user = "usernameKey"
        case password = "passwordKey"
        case ssl = "sslKey"
        case rememberMe = "rememberKey"
    }
}

class LoginViewModel: ObservableObject {
    @Published var username = ""
    @Published var password = ""
    @Published var loading = false
    @Published var inputEnabled = false

    @Published var rememberMe = false {
        didSet { if rememberMe == false { initialize() } }
    }

    @Published var connecting: Bool = false {
        didSet {
            loading = connecting
            inputEnabled = !connecting
        }
    }

    var isSigninComplete: Bool {
        if !isUsernameValid() || !isPasswordValid() {
            return false
        }
        return true
    }

    init() {
        configureDefaultValues()
    }

    // Validation methods
    private func isUsernameValid() -> Bool {
        return username != ""
    }

    private func isPasswordValid() -> Bool {
        return true
    }

    private func initialize() {
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

    public func storeDefaultValues() {
        UserDefaults.standard.set(username, forKey: Configuration.Keys.user.rawValue)
        UserDefaults.standard.set(password, forKey: Configuration.Keys.password.rawValue)
    }
}
