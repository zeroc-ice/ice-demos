// Copyright (c) ZeroC, Inc.

using Demo;

internal class HelloI(string name) : HelloDisp_
{
    public override string getGreeting(Ice.Current current)
    {
        return _name + " says Hello World!";
    }

    public override void shutdown(Ice.Current current)
    {
        System.Console.Out.WriteLine("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }

    private string _name = name;
}
