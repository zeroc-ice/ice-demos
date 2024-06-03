// Copyright (c) ZeroC, Inc.

using Demo;

/// <summary>Implements Slice interface `Hello` by printing messages.</summary>
public class Printer : HelloDisp_
{
    /// <summary>Prints a message to the standard output.</summary>
    /// <param name="current">The current object of the corresponding incoming request.</param>
    public override void sayHello(Ice.Current current)
    {
        Console.WriteLine("Hello World!");
    }
}
