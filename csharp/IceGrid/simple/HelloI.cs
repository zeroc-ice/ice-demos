// Copyright (c) ZeroC, Inc.

using Demo;

public class HelloI(string name) : HelloDisp_
{
    public override void sayHello(Ice.Current current)
    {
        Console.WriteLine(_name + " says Hello World!");
    }

    public override void shutdown(Ice.Current current)
    {
        Console.WriteLine(_name + " shutting down...");
        current.adapter.getCommunicator().shutdown();
    }

    private string _name = name;
}
