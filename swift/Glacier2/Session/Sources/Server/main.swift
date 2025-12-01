// Copyright (c) ZeroC, Inc.

import Foundation
import Glacier2
import Ice

@main
struct Server {
    static func main() async throws {
        // Create an Ice communicator. We'll use this communicator to create an object adapter.
        let communicator = try Ice.initialize(CommandLine.arguments)
        defer {
            communicator.destroy()
        }

        // Set up signal handler to shut down cleanly on Ctrl+C
        signal(SIGINT) { _ in
            communicator.shutdown()
        }

        // Create an object adapter that listens for incoming requests and dispatches them to servants.
        let adapter = try communicator.createObjectAdapterWithEndpoints(
            name: "PokeAdapter",
            endpoints: "tcp -p 4061"
        )

        // Register the SessionManager servant with the adapter.
        let sessionManager = SessionManager(adapter: adapter)
        try adapter.add(
            servant: Ice.Disp(sessionManager),
            id: Ice.Identity(name: "SessionManager", category: "")
        )

        // Register the SharedPokeBox servant with the adapter as a default servant for category "PokeBox".
        // This servant implements all PokeBox objects.
        let sharedPokeBox = SharedPokeBox(
            pokeStore: InMemoryPokeStore(),
            userIdResolver: sessionManager
        )
        try adapter.addDefaultServant(servant: Ice.Disp(sharedPokeBox), category: "PokeBox")

        // Start dispatching requests.
        try adapter.activate()
        print("Listening on port 4061...")

        // Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
        communicator.waitForShutdown()
    }
}
