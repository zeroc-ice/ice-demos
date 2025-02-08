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
        string greet(string name);
    }
}
