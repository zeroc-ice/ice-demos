// Copyright (c) ZeroC, Inc.

using Demo;

internal class HelloI : HelloDisp_
{
    public override void sayHello(int delay, Ice.Current current)
    {
        if (delay > 0)
        {
            System.Threading.Thread.Sleep(delay);
        }
        Console.Out.WriteLine("Hello World!");
    }

    public override void shutdown(Ice.Current current)
    {
        Console.Out.WriteLine("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
