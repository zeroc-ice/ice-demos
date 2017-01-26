// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public final class CallbackI extends _CallbackDisp
{
    @Override
    public void
    initiateCallback(CallbackReceiverPrx proxy, Ice.Current current)
    {
        System.out.println("initiating callback to: " + current.adapter.getCommunicator().proxyToString(proxy));
        try
        {
            proxy.callback();
        }
        catch(Ice.LocalException ex)
        {
            ex.printStackTrace();
        }
    }

    @Override
    public void
    shutdown(Ice.Current current)
    {
        System.out.println("shutting down...");
        try
        {
            current.adapter.getCommunicator().shutdown();
        }
        catch(Ice.LocalException ex)
        {
            ex.printStackTrace();
        }
    }
}
