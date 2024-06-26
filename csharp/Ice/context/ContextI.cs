// Copyright (c) ZeroC, Inc.

using Demo;

internal class ContextI : ContextDisp_
{
    public override void call(Ice.Current current)
    {
        Console.Out.Write("Type = ");
        if (current.ctx.ContainsKey("type"))
        {
            Console.Out.Write(current.ctx["type"]);
        }
        else
        {
            Console.Out.Write("None");
        }
        Console.Out.WriteLine("");
    }

    public override void shutdown(Ice.Current current)
    {
        Console.Out.WriteLine("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
