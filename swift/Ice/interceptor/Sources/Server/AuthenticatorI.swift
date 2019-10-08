//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice
import Security

class AuthenticatorI: Authenticator {
    private var _tokenStore: [String: Date] = [:]

    func getToken(current _: Ice.Current) -> String {
        //
        // Generate a random 32 character long token.
        //
        let chars = Array("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
        var bytes = [UInt8](repeating: 0, count: 32)
        guard SecRandomCopyBytes(kSecRandomDefault, bytes.count, &bytes) == errSecSuccess else {
            print("Failed to generate token.")
            return ""
        }

        var token = ""
        for byte in bytes {
            token.append(chars[Int(byte) % chars.count])
        }

        //
        // By default tokens are valid for 30 seconds after being issued.
        //
        let expireTime = Date(timeIntervalSinceNow: TimeInterval(30.0))
        _tokenStore.updateValue(expireTime, forKey: token)

        return token
    }

    func validateToken(tokenValue: String) throws {
        //
        // Remove any expired tokens.
        //
        let currentTime = Date()
        _tokenStore = _tokenStore.filter { $0.value > currentTime }

        //
        // We assume if the client passed a token, but there's no
        // stored values matching it, that it must of expired.
        //
        guard (_tokenStore.contains { $0.key == tokenValue }) else {
            throw TokenExpiredException()
        }
    }
}
