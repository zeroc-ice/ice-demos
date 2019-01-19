//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using System;
using Demo;

public class HelloI : HelloDisp_
{
    public override void sayHello(Ice.Current current)
    {
        Console.Out.WriteLine("Hello World!");
    }
}
