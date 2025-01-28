// Copyright (c) ZeroC, Inc.

import Ice
import SwiftUI

let hostnameKey = "hostnameKey"

struct FormView: View {
    @StateObject var client = Client()

    @State var statusBarHidden = true
    @State var statusBarText = ""

    var communicator: Ice.Communicator!

    var body: some View {
        ZStack {
            Form {
                // Proxy Settings
                ProxySettingsView(connection: $client.proxySettings.connection,
                                  delay: $client.proxySettings.delay,
                                  timeout: $client.proxySettings.timeout,
                                  methodIndex: $client.proxySettings.methodIndex).environmentObject(client)

                // Buttons
                Section("Actions") {
                    ButtonView(helloEnabled: $client.helloEnabled,
                               flushEnabled: $client.flushEnabled,
                               shutdownEnabled: $client.shutdownEnabled)
                        .environmentObject(client)
                }

                Section("Status message") {
                    // Status view
                    StatusView(isSpinning: $client.isSpinning, text: $client.statusMessage)
                }
            }
        }
        // Displaying Alerts
        .alert(isPresented: $client.showingError) {
            Alert(title: Text("Error"),
                  message: Text(client.error?.localizedDescription ?? ""),
                  dismissButton: .default(Text("Got it!")))
        }
    }
}

extension Binding {
    func didSet(_ didSet: @escaping (Value) -> Void) -> Binding<Value> {
        Binding(
            get: { wrappedValue },
            set: { newValue in
                self.wrappedValue = newValue
                didSet(newValue)
            }
        )
    }
}
