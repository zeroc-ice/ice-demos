// Copyright (c) ZeroC, Inc.

import SwiftUI

struct ContentView: View {
    @EnvironmentObject var client: GreeterClient
    @State private var host = "localhost"
    @State private var user = "SwiftUI"
    @State private var greeting = ""
    @State private var error: String?
    @State private var isLoading = false

    var body: some View {
        Form {
            Section {
                HStack {
                    Text("Host:")
                    TextField("e.g. localhost", text: $host)
                        .textContentType(.URL)
                        .disableAutocorrection(true)
                        .disabled(isLoading)
                }

                HStack {
                    Text("Name:")
                    TextField("Your name", text: $user)
                        .textContentType(.name)
                        .disabled(isLoading)
                }
            } header: {
                Text("Connection Settings")
            }
            
            Section {
                HStack {
                    Spacer()
                    ZStack {
                        Button("Greet") {
                            // Start the task in the main actor because it updates the UI.
                            Task { @MainActor in
                                // Reset state before making the request
                                greeting = ""
                                error = nil
                                isLoading = true
                                defer {
                                    isLoading = false
                                }
                                do {
                                    greeting = try await client.greet(user: user, host: host)
                                } catch {
                                    self.error = "Error: \(error)"
                                }
                            }
                        }.disabled(isLoading)
                        
                        if isLoading {
                            ProgressView()
                                .progressViewStyle(CircularProgressViewStyle())
                                .allowsHitTesting(false)
                        }
                    }
                    Spacer()
                }
            }
            
            if !greeting.isEmpty || error != nil {
                Section("Result") {
                    if let error = error {
                        Text(error)
                            .foregroundColor(.red)
                            .font(.body)
                            .multilineTextAlignment(.leading)
                    } else {
                        Text(greeting)
                            .font(.subheadline)
                    }
                }
            }
        }
    #if os(macOS)
   .formStyle(.grouped) // or .columns (macOS 14+)
   .padding()
   .frame(minWidth: 600, idealWidth: 600, maxWidth: 600)
   #endif
    }
}

#Preview {
    ContentView()
}
