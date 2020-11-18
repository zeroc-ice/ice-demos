// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading;
using Demo;

public class HelloI : HelloDisp_
{
    public override void
    sayHello(Ice.Current current, CancellationToken cancel)
    {
        System.Console.Out.WriteLine("Hello World!");
    }
}
