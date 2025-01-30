// Copyright (c) ZeroC, Inc.

#pragma once

module VisitorCenter
{
    /// Represents a simple greeter.
    interface Greeter
    {
        /// Creates a personalized greeting.
        /// @param name: The name of the person to greet.
        /// @returns: The greeting.
        ["amd", "cs:identifier:Greet"] // Asynchronous Method Dispatch and remapped method name
        string greet(string name);
    }
}
