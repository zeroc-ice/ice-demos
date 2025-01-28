// Copyright (c) ZeroC, Inc.

import SwiftUI

@main
struct HelloApp: App {
    var body: some Scene {
        WindowGroup {
            #if os(iOS)
                NavigationView {
                    FormView()
                }
                .navigationViewStyle(.stack)
            #else
                FormView()
            #endif
        }
    }
}
