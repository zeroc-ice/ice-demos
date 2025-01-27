// Copyright (c) ZeroC, Inc.

module VisitorCenter
{
    /// Represents a simple greeter.
    interface Greeter
    {
        /// Creates a personalized greeting.
        /// @param name: The name of the person to greet.
        /// @returns: The greeting.
        ["amd"] // Instructs the Slice compiler to generate "asynchronous method dispatch" support code.
        string greet(string name);
    }
}
