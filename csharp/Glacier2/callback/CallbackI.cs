// Copyright (c) ZeroC, Inc.

using Demo;

public sealed class CallbackI : CallbackDisp_
{
    public override void initiateCallback(CallbackReceiverPrx proxy, Ice.Current current)
    {
        Console.WriteLine("initiating callback to: " + current.adapter.getCommunicator().proxyToString(proxy));
        try
        {
            proxy.callback();
        }
        catch (System.Exception ex)
        {
            Console.Error.WriteLine(ex);
        }
    }

    public override void shutdown(Ice.Current current)
    {
        Console.WriteLine("shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
