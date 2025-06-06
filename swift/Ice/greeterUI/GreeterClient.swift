// Copyright (c) ZeroC, Inc.

import Combine
import Ice
import SwiftUI

class GreeterClient: ObservableObject {
    // MARK: - Published Properties
    
    @Published var serverAddress = "localhost:4061"
    @Published var userName = ""
    @Published var greetingResponse = ""
    @Published var isLoading = false
    @Published var errorMessage = ""
    @Published var showingError = false
    
    // MARK: - Private Properties
    
    private var communicator: Ice.Communicator
    
    // MARK: - Initialization
    
    init() {
        do {
            communicator = try Ice.initialize()
        } catch {
            // Handle initialization error
            communicator = try! Ice.initialize() // Fallback
        }
    }
    
    deinit {
        communicator.destroy()
    }
    
    // MARK: - Public Methods
    
    func sendGreeting() {
        guard !userName.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty else {
            showError("Please enter your name")
            return
        }
        
        isLoading = true
        errorMessage = ""
        greetingResponse = ""
        
        Task {
            do {
                // Parse server address to extract host and port
                let components = serverAddress.split(separator: ":")
                let host = components.first.map(String.init) ?? "localhost"
                let port = components.count > 1 ? String(components[1]) : "4061"
                
                let proxyString = "greeter:tcp -h \(host) -p \(port)"
                let greeter = try makeProxy(
                    communicator: communicator,
                    proxyString: proxyString,
                    type: GreeterPrx.self
                )
                
                let greeting = try await greeter.greet(userName.trimmingCharacters(in: .whitespacesAndNewlines))
                
                await MainActor.run {
                    self.greetingResponse = greeting
                    self.isLoading = false
                }
            } catch {
                await MainActor.run {
                    self.isLoading = false
                    self.showError("Failed to send greeting: \(error.localizedDescription)")
                }
            }
        }
    }
    
    private func showError(_ message: String) {
        errorMessage = message
        showingError = true
    }
}