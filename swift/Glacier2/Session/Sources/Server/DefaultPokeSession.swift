// Copyright (c) ZeroC, Inc.

import Foundation
import Glacier2
import Ice

/// DefaultPokeSession is an Ice servant that implements Slice interface PokeSession. We create a
/// DefaultPokeSession for each PokeSession object.
class DefaultPokeSession: CatchThemAll.PokeSession {
    private let adapter: Ice.ObjectAdapter
    private let sessionControl: Glacier2.SessionControlPrx
    private let userIdResolver: UserIdResolver

    /// Constructs a DefaultPokeSession servant.
    /// - Parameters:
    ///   - adapter: The object adapter that hosts this servant and the PokeBox objects.
    ///   - sessionControl: The session control proxy.
    ///   - userIdResolver: The user ID resolver.
    init(
        adapter: Ice.ObjectAdapter,
        sessionControl: Glacier2.SessionControlPrx,
        userIdResolver: UserIdResolver
    ) {
        self.adapter = adapter
        self.sessionControl = sessionControl
        self.userIdResolver = userIdResolver
    }

    func destroy(current: Ice.Current) async throws {
        print("Destroying session #\(current.id.name)")

        // Remove the token from the user ID resolver, since the token is no longer valid.
        userIdResolver.removeToken(current.id.name)

        // Remove this servant from the object adapter. A new call to this session object will fail with
        // ObjectNotExistException.
        try adapter.remove(id: current.id)

        // Destroy the session in the Glacier2 router.
        try await sessionControl.destroy()
    }

    func getPokeBox(current: Ice.Current) async throws -> CatchThemAll.PokeBoxPrx? {
        // The session token is the name component of the session identity; we use it for the identity of the PokeBox
        // object as well.
        return try adapter.createProxy(
            id: Ice.Identity(name: current.id.name, category: "PokeBox"),
            type: CatchThemAll.PokeBoxPrx.self
        )
    }
}
