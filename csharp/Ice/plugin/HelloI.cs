// Copyright (c) ZeroC, Inc.

using Demo;

public class HelloI : HelloDisp_
{
    public override void sayHello(Ice.Current current)
    {
        current.adapter.getCommunicator().getLogger().print("Hello World!");
    }

    public override void shutdown(Ice.Current current)
    {
        current.adapter.getCommunicator().getLogger().print("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
