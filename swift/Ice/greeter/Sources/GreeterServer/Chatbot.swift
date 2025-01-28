// Copyright (c) ZeroC, Inc.

import Ice

class ChatBot: Greeter {
    func greet(name: String, current _: Ice.Current) throws -> String {
        print("Dispatching greet request { name = '\(name)' }")
        return "Hello, \(name)!"
    }
}
