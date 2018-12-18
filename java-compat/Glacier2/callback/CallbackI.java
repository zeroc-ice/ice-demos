// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
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
        current.adapter.getCommunicator().shutdown();
    }
}
