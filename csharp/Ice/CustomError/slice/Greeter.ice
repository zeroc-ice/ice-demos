// Copyright (c) ZeroC, Inc.

#pragma once

module VisitorCenter
{
    /// Represents the error code carried by GreeterException.
    enum GreeterError { NameTooLong, Away, GreetingOtherVisitor };

    /// Represents the custom error returned by the Greeter when it cannot create a greeting. This exception thrown by
    /// the implementation of the greet operation and caught by caller.
    exception GreeterException {
        /// Describes the exception in a human-readable way.
        ["cs:identifier:ErrorMessage"] // We can't use Message as it conflicts with System.Exception.Message.
        string message;

        /// Provides an error code that can be used to handle this exception programmatically.
        ["cs:identifier:Error"]
        GreeterError error;
    }

    /// Represents a simple greeter.
    interface Greeter
    {
        /// Creates a personalized greeting.
        /// @param name The name of the person to greet.
        /// @return The greeting.
        /// @throws GreeterException Thrown when the greeter cannot create a greeting.
        ["cs:identifier:Greet"] // We prefer PascalCase for C# methods.
        string greet(string name)
            throws GreeterException;
    }
}
