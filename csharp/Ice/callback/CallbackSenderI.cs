// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;

public sealed class CallbackSenderI : CallbackSenderDisp_
{
    public override void initiateCallback(CallbackReceiverPrx proxy, Ice.Current current)
    {
        Console.Out.WriteLine("initiating callback");
        try
        {
            proxy.callback();
        }
        catch(Exception ex)
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
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
        }
    }
}
