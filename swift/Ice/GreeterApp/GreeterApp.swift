// Copyright (c) ZeroC, Inc.

import Ice
import SwiftUI

@main
struct GreeterApp: App {
    // Holds the GreeterClient, which manages the Ice communicator and sends requests to the Greeter service.
    @StateObject private var client = GreeterClient()

    var body: some Scene {
        WindowGroup {
            // Injects the GreeterClient into GreeterView via the environment.
            GreeterView().environmentObject(client)
        }
        #if os(macOS)
            .windowResizability(.contentSize)
        #endif
    }
}
