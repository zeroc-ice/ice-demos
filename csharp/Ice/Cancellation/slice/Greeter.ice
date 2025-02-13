// Copyright (c) ZeroC, Inc.

#pragma once

module VisitorCenter
{
    /// Represents a simple greeter.
    interface Greeter
    {
        /// Creates a personalized greeting.
        /// @param name The name of the person to greet.
        /// @return The greeting.
        ["amd"] // Asynchronous Method Dispatch.
        ["cs:identifier:Greet"] // We prefer PascalCase for C# methods.
        string greet(string name);
    }

    /// Represents the administrative interface for a Greeter.
    interface GreeterAdmin
    {
        /// Pauses the greeter. While paused, the greet operation blocks and does not return any greeting until after
        /// {@link resume} is called.
        ["cs:identifier:Pause"]
        void pause();

        /// Resumes the greeter.
         ["cs:identifier:Resume"]
        void resume();
    }
}
