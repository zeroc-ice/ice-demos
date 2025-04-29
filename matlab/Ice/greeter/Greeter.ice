// Copyright (c) ZeroC, Inc.

#pragma once

// We use matlab:identifier to get a lowercase namespace in MATLAB; without this metadata directive, the mapped
// namespace is VisitorCenter.
["matlab:identifier:visitorcenter"]
module VisitorCenter
{
    /// Represents a simple greeter.
    interface Greeter
    {
        /// Creates a personalized greeting.
        /// @param name The name of the person to greet.
        /// @return The greeting.
        string greet(string name);
    }
}
