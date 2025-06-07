// Copyright (c) ZeroC, Inc.

import SwiftUI

struct GreeterView: View {
    @StateObject private var client = GreeterClient()
    
    var body: some View {
        Form {
            Section {
                TextField("Server Address", text: $client.serverAddress)
                    .autocapitalization(.none)
                    .disableAutocorrection(true)
                
                TextField("Your Name", text: $client.userName)
                    .autocapitalization(.words)
            } header: {
                Text("Connection Settings")
            } footer: {
                Text("Enter the server address and port (e.g., localhost:4061) for the Greeter service")
            }
            
            Section {
                Button(action: {
                    client.sendGreeting()
                }) {
                    HStack {
                        if client.isLoading {
                            ProgressView()
                                .scaleEffect(0.8)
                        }
                        Text("Send Greeting")
                    }
                }
                .disabled(client.isLoading || client.userName.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty)
            } header: {
                Text("Action")
            }
            
            if !client.greetingResponse.isEmpty {
                Section {
                    Text(client.greetingResponse)
                        .textSelection(.enabled)
                } header: {
                    Text("Response")
                }
            }
        }
        .navigationTitle("Ice Greeter")
        .alert("Error", isPresented: $client.showingError) {
            Button("OK") { }
        } message: {
            Text(client.errorMessage)
        }
        .onAppear {
            // Set default name based on current user if empty
            if client.userName.isEmpty {
                #if os(macOS)
                client.userName = NSUserName()
                #else
                client.userName = "Swift User"
                #endif
            }
        }
    }
}
