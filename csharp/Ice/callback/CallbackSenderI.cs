// Copyright (c) ZeroC, Inc.

using Demo;

public sealed class CallbackSenderI : CallbackSenderDisp_
{
    public override void initiateCallback(CallbackReceiverPrx proxy, Ice.Current current)
    {
        Console.Out.WriteLine("initiating callback");
        try
        {
            proxy.callback();
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
        }
    }

    public override void shutdown(Ice.Current current)
    {
        Console.Out.WriteLine("Shutting down...");
        try
        {
            current.adapter.getCommunicator().shutdown();
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
        }
    }
}
