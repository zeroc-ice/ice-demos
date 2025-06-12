// Copyright (c) ZeroC, Inc.

import SwiftUI

// GreeterView is the main view of the application.
// It lets the user enter their name and the host address of the Greeter service,
// and sends a greeting request when the user clicks the "Greet" button.
struct GreeterView: View {
    // The GreeterClient is managed by GreeterApp and provided via the environment.
    @EnvironmentObject var client: GreeterClient

    // The target host defaults to "localhost".
    @State private var host = "localhost"

    // The user name defaults to "Alice" on iOS and the current user's name on macOS.
    #if os(macOS)
        @State private var user = NSUserName()
    #else
        @State private var user = "Alice"
    #endif

    // The greeting message, if one has been received; empty otherwise.
    @State private var greeting = ""
    // An error message if an error occurred during the request.
    @State private var error: String?
    // A loading state to indicate that a request is in progress.
    @State private var isLoading = false

    var body: some View {
        Form {
            // The Settings section lets the user enter the host and their name.
            // On iOS, we add explicit labels for accessibility; on macOS, we rely on default field labels.
            Section("Settings") {
                HStack {
                    #if os(iOS)
                        Text("Host:")
                    #endif
                    TextField("Host:", text: $host, prompt: Text("e.g. localhost"))
                        .textContentType(.URL)
                        .disableAutocorrection(true)
                        .disabled(isLoading)
                }

                HStack {
                    #if os(iOS)
                        Text("Name:")
                    #endif
                    TextField("Name:", text: $user, prompt: Text("e.g. Alice"))
                        .textContentType(.name)
                        .disabled(isLoading)
                }
            }

            Section {
                HStack {
                    Spacer()
                    // We use a ZStack to overlay a ProgressView on top of the Greet button when loading.
                    ZStack {
                        Button("Greet") {
                            // Start the task in the main actor because it updates the UI.
                            Task { @MainActor in
                                // Reset state before making the request.
                                greeting = ""
                                error = nil
                                // Set loading state before sending the request.
                                isLoading = true
                                defer {
                                    // Reset loading state after the request completes.
                                    isLoading = false
                                }
                                do {
                                    // Send the greeting request and await the response.
                                    greeting = try await client.greet(user: user, host: host)
                                } catch {
                                    self.error = "Error: \(error)"
                                }
                            }
                        }.disabled(isLoading)

                        if isLoading {
                            // Show a circular progress indicator while loading.
                            ProgressView()
                                .progressViewStyle(CircularProgressViewStyle())
                                .allowsHitTesting(false)
                        }
                    }
                    Spacer()
                }
            }

            // Display either the greeting or the error message.
            if !greeting.isEmpty || error != nil {
                Section {
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
        // Sets the padding and frame size to improve appearance on macOS.
        #if os(macOS)
            .padding()
            .frame(
                minWidth: 600, idealWidth: 600, maxWidth: 600,
                minHeight: 500, idealHeight: 500, maxHeight: 500)
        #endif
    }
}

#Preview {
    GreeterView()
}
