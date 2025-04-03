// Copyright (c) ZeroC, Inc.

using VisitorCenter;

namespace Server;

/// <summary>Chatbot is an Ice servant that implements Slice interface Greeter.</summary>
internal class Chatbot : GreeterDisp_
{
    /// <inheritdoc/>
    // Implements the abstract method Greet from the GreeterDisp_ class generated by the Slice compiler.
    public override string Greet(string name, Ice.Current current)
    {
        Console.WriteLine($"Dispatching greet request {{ name = '{name}' }}");
        return $"Hello, {name}!";
    }
}
