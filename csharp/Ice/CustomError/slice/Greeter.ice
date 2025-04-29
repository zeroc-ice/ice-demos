// Copyright (c) ZeroC, Inc.

#pragma once

module VisitorCenter
{
    /// The errors returned by the Greeter.
    enum GreeterError { EmptyName, NameTooLong, Away };

    /// Represents an exception.
    exception GreeterException {
        /// The reason for the exception.
        string reason;

        //The ID for the exception, given as a GreeterError enum.
        GreeterError error;
    }

    /// Represents a simple greeter.
    interface Greeter
    {
        /// Creates a personalized greeting.
        /// @param name The name of the person to greet.
        /// @return The greeting or exception.
        ["cs:identifier:Greet"] // We prefer PascalCase for C# methods.
        string greet(string name) throws GreeterException;
    }
}
