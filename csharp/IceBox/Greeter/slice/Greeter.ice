// Copyright (c) ZeroC, Inc.

module VisitorCenter
{
    /// Represents a simple greeter.
    interface Greeter
    {
        /// Creates a personalized greeting.
        /// @param name The name of the person to greet.
        /// @return The greeting.
        ["cs:identifier:Greet"] // We prefer PascalCase for C# methods.
        string greet(string name);
    }
}
