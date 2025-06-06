// Copyright (c) ZeroC, Inc.

import SwiftUI

@main
struct GreeterApp: App {
    var body: some Scene {
        WindowGroup {
            #if os(iOS)
                NavigationView {
                    GreeterView()
                }
                .navigationViewStyle(.stack)
            #else
                GreeterView()
            #endif
        }
    }
}