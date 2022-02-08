//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

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
                ProxySettingsView(hostname: $client.proxySettings.hostname,
                                  useDiscovery: $client.proxySettings.useDiscovery,
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
        .onAppear {
            client.configure(comm: communicator)
        }
    }
}

struct FormView_Previews: PreviewProvider {
    static var previews: some View {
        Group {
            ContentView()
                .previewDevice("iPhone 13 mini")
                .previewDisplayName("iPhone 13 mini")
                .previewInterfaceOrientation(.portrait)
            ContentView()
                .previewDevice("iPhone 13 Pro")
                .previewDisplayName("iPhone 13 Pro")
                .previewInterfaceOrientation(.portrait)
            ContentView()
                .previewDevice("iPhone 13 Pro Max")
                .previewDisplayName("iPhone 13 Pro Max")
                .previewInterfaceOrientation(.portrait)
            ContentView()
                .previewDevice("iPad Air (4th generation)")
                .previewDisplayName("iPad Air")
                .previewInterfaceOrientation(.landscapeLeft)
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
