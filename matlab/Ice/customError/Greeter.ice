// Copyright (c) ZeroC, Inc.

["matlab:identifier:visitorcenter"]
module VisitorCenter
{
    /// Represents the error code carried by GreeterException.
    enum GreeterError { NameTooLong, Away, GreetingOtherVisitor }

    /// Represents the custom error returned by the Greeter when it cannot create a greeting. This exception is thrown
    /// by the implementation of the greet operation and caught by the caller.
    exception GreeterException
    {
        /// Describes the exception in a human-readable way.
        ["matlab:identifier:Message"] // We prefer Pascal case for MATLAB properties.
        string message;

        /// Provides an error code that can be used to handle this exception programmatically.
        ["matlab:identifier:Error"]
        GreeterError error;
    }

    /// Represents a simple greeter.
    interface Greeter
    {
        /// Creates a personalized greeting.
        /// @param name The name of the person to greet.
        /// @return The greeting.
        /// @throws GreeterException Thrown when the greeter cannot create a greeting.
        string greet(string name) throws GreeterException;
    }
}
