//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using System;
using Demo;

/// <summary>Implements Slice interface `Hello` by printing messages.</summary>
public class Printer : HelloDisp_
{
    /// <summary>Prints a message to the standard output.</summary>
    public override void sayHello(Ice.Current current)
    {
        Console.WriteLine("Hello World!");
    }
}
