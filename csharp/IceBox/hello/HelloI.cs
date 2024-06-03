// Copyright (c) ZeroC, Inc.

using Demo;

internal class HelloI : HelloDisp_
{
    public override void sayHello(Ice.Current current)
    {
        System.Console.Out.WriteLine("Hello World!");
    }
}
