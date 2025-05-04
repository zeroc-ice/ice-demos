// Copyright (c) ZeroC, Inc.

#pragma once

module VisitorCenter
{
    /// This enum is used to specify the underlying cause of a GreeterException.
    enum GreeterError { EmptyName, NameTooLong, Away };

    /// Represents an exception.
    /// An exception will be thrown in one of three circumstances depending on the name Greeter is given:
    /// 1) The name is empty
    /// 2) The name is longer than a specified maximum length
    /// 3) The name is associated with an individual who is listed as being "away"
    exception GreeterException {
        /// The reason for the exception.
        string message;

        /// The ID for the exception.
        GreeterError error;
    }

    /// Represents a simple greeter.
    interface Greeter
    {
        /// Creates a personalized greeting.
        /// @param name The name of the person to greet.
        /// @return The greeting.
        /// @throws GreeterException Thrown when the name matches an away name, is too long, or is empty.
        ["cs:identifier:Greet"] // We prefer PascalCase for C# methods.
        string greet(string name) throws GreeterException;
    }
}
