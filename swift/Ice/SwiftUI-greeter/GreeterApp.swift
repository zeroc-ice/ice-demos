// Copyright (c) ZeroC, Inc.

import SwiftUI
import Ice

@main
struct SwiftUI_GreeterApp: App {
    @StateObject private var client = GreeterClient()
    
    var body: some Scene {
        WindowGroup {
            ContentView().environmentObject(client)
        }
        #if os(macOS)
        .windowResizability(.contentSize) 
        #endif
    }
}
