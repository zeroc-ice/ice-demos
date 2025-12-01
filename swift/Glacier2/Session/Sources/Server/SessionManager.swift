// Copyright (c) ZeroC, Inc.

import Foundation
import Glacier2
import Ice

/// SessionManager is an Ice servant that implements Slice interface Glacier2::SessionManager. It creates sessions
/// and resolves user IDs from session tokens.
/// Note: This demo implementation is not thread-safe. A real implementation should support concurrent calls.
class SessionManager: Glacier2.SessionManager, UserIdResolver {
    private let adapter: Ice.ObjectAdapter
    private var tokenToUserId: [String: String] = [:]

    /// Constructs a SessionManager servant.
    /// - Parameter adapter: The object adapter that hosts the session servants.
    init(adapter: Ice.ObjectAdapter) {
        self.adapter = adapter
    }

    func create(
        userId: String,
        control: Glacier2.SessionControlPrx?,
        current: Ice.Current
    ) async throws -> Glacier2.SessionPrx? {
        // control is not nil because we configured Glacier2.Server.Endpoints in the Glacier2 router
        // configuration file.
        guard let sessionControl = control else {
            fatalError("SessionControl is nil")
        }

        // Create a new session servant and add it to the adapter with a UUID identity. The name component of the
        // identity is the session token.
        let sessionServant = DefaultPokeSession(
            adapter: adapter,
            sessionControl: sessionControl,
            userIdResolver: self
        )

        let proxy = try adapter.addWithUUID(servant: Ice.Disp(sessionServant)) as CatchThemAll.PokeSessionPrx

        let sessionToken = proxy.ice_getIdentity().name
        tokenToUserId[sessionToken] = userId

        print("Created session #\(sessionToken) for user '\(userId)'")
        return proxy
    }

    func getUserId(_ token: String) -> String? {
        return tokenToUserId[token]
    }

    func removeToken(_ token: String) {
        tokenToUserId.removeValue(forKey: token)
    }
}
