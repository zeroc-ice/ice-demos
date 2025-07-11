// Copyright (c) ZeroC, Inc.

import Ice

/// Chatbot is an Ice servant that implements Slice interface Greeter.
struct Chatbot: Greeter {
    private let greeterName: String

    /// Constructs a Chatbot servant.
    /// - Parameter greeterName: The name of the greeter.
    init(greeterName: String) {
        self.greeterName = greeterName
    }

    // Implements the method greet from the Greeter protocol generated by the Slice compiler.
    func greet(name: String, current _: Ice.Current) -> String {
        print("Dispatching greet request { name = '\(name)' }")
        return "Hello, \(name)! My name is \(greeterName). How are you doing today?"
    }
}
