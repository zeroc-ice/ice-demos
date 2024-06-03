// Copyright (c) ZeroC, Inc.

using Demo;

public class HelloI : HelloDisp_
{
    public override void sayHello(Ice.Current current)
    {
        System.Console.Out.WriteLine("Hello World!");
    }
}
