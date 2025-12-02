// Copyright (c) ZeroC, Inc.

import Glacier2
import Ice

/// DefaultPokeSession is an Ice servant that implements Slice interface PokeSession. We create a DefaultPokeSession
/// for each PokeSession object.
struct DefaultPokeSession: PokeSession {
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
        await userIdResolver.removeToken(current.id.name)

        // Remove this servant from the object adapter. A new call to this session object will fail with
        // ObjectNotExistException.
        try adapter.remove(current.id)

        // Destroy the session in the Glacier2 router.
        do {
            try await sessionControl.destroy()
        } catch is ObjectNotExistException {
            // The session was already destroyed, ignore.
        }
    }

    func getPokeBox(current: Ice.Current) throws -> PokeBoxPrx? {
        // The session token is the name component of the session identity; we use it for the identity of the PokeBox
        // object as well.
        try uncheckedCast(
            prx: adapter.createProxy(Ice.Identity(name: current.id.name, category: "PokeBox")),
            type: PokeBoxPrx.self)
    }
}
