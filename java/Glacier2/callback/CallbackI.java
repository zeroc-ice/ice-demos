// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public final class CallbackI implements Callback
{
    @Override
    public void initiateCallback(CallbackReceiverPrx proxy, com.zeroc.Ice.Current current)
    {
        System.out.println("initiating callback to: " + current.adapter.getCommunicator().proxyToString(proxy));
        try
        {
            proxy.callback();
        }
        catch(com.zeroc.Ice.LocalException ex)
        {
            ex.printStackTrace();
        }
    }

    @Override
    public void shutdown(com.zeroc.Ice.Current current)
    {
        System.out.println("shutting down...");
        try
        {
            current.adapter.getCommunicator().shutdown();
        }
        catch(com.zeroc.Ice.LocalException ex)
        {
            ex.printStackTrace();
        }
    }
}
