// Copyright (c) ZeroC, Inc.

#pragma once

module VisitorCenter
{
    /// This enum is used as a field of GreeterException, functionally as an error ID.
    enum GreeterError { EmptyName, NameTooLong, Away };

    /// Represents an exception.
    /// It contains a `string` message and `GreeterError` error.
    /// The message is used by any application attempting to throw an exception to describe the nature of the exception.
    /// The error is used similarly, but is limited to the enums in GreeterError and is better for internal identifying.
    exception GreeterException {
        /// The reason for the exception.
        string message;

        /// The ID for the exception, given as a GreeterError enum.
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
