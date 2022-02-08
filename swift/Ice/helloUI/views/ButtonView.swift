//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import SwiftUI

struct ButtonView: View {
    @EnvironmentObject var client: Client

    @Binding var helloEnabled: Bool
    @Binding var flushEnabled: Bool
    @Binding var shutdownEnabled: Bool

    var body: some View {
        HStack(alignment: .center, spacing: 2.0) {
            Button("Say Hello") {
                client.sayHello()
            }.disabled(!helloEnabled)
            Spacer()
            Button("Flush") {
                client.flushBatch()
            }.disabled(!flushEnabled)
            Spacer()
            Button("Shutdown") {
                client.shutdown()
            }.disabled(!shutdownEnabled)
        }.buttonStyle(.bordered)
            .navigationTitle("Hello World!")
    }
}
