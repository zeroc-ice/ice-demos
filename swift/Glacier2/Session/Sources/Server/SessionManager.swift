// Copyright (c) ZeroC, Inc.

import Glacier2
import Ice

/// SessionManager is an Ice servant that implements Slice interface Glacier2::SessionManager. It creates sessions
/// and resolves user IDs from session tokens.
actor SessionManager: Glacier2.SessionManager, UserIdResolver {
    private let adapter: Ice.ObjectAdapter
    private var tokenToUserId: [String: String] = [:]

    /// Constructs a SessionManager servant.
    /// - Parameter adapter: The object adapter that hosts the session servants.
    init(adapter: Ice.ObjectAdapter) {
        self.adapter = adapter
    }

    func create(userId: String, control: Glacier2.SessionControlPrx?, current: Ice.Current) throws -> Glacier2.SessionPrx? {
        // control is not nil because we configured Glacier2.Server.Endpoints in the Glacier2 router
        // configuration file.
        guard let sessionControl = control else {
            fatalError("the SessionControl proxy is nil")
        }

        // Create a new session servant and add it to the adapter with a UUID identity. The name component of the
        // identity is the session token.
        let sessionServant = DefaultPokeSession(
            adapter: adapter,
            sessionControl: sessionControl,
            userIdResolver: self
        )

        let proxy = try uncheckedCast(prx: adapter.addWithUUID(sessionServant), type: PokeSessionPrx.self)

        let sessionToken = proxy.ice_getIdentity().name
        tokenToUserId[sessionToken] = userId

        print("Created session #\(sessionToken) for user '\(userId)'")
        return proxy
    }

    func getUserId(_ token: String) -> String? {
        tokenToUserId[token]
    }

    func removeToken(_ token: String) {
        tokenToUserId.removeValue(forKey: token)
    }
}
