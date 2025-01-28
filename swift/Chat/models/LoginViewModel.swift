// Copyright (c) ZeroC, Inc.

import Combine
import Glacier2
import Ice
import PromiseKit
import SwiftUI

class LoginViewModel: ObservableObject {
    @Published var username = ""
    @Published var password = ""
    @Published var loading = false
    @Published var inputEnabled = false

    @Published var connecting: Bool = false {
        didSet {
            loading = connecting
            inputEnabled = !connecting
        }
    }

    var isSigninComplete: Bool {
        if !isUsernameValid() {
            return false
        }
        return true
    }

    // Validation methods
    private func isUsernameValid() -> Bool {
        return username != ""
    }
}
