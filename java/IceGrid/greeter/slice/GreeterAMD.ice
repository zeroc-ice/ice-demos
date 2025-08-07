// Copyright (c) ZeroC, Inc.

#pragma once

["java:identifier:com.example.visitorcenter"]
module VisitorCenter
{
    /// Represents a simple greeter.
    interface Greeter
    {
        /// Creates a personalized greeting.
        /// @param name The name of the person to greet.
        /// @return The greeting.
        ["amd"] // Asynchronous Method Dispatch.
        string greet(string name);
    }
}
