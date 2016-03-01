// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public final class CallbackSenderI extends _CallbackSenderDisp
{
    @Override
    public void
    initiateCallback(CallbackReceiverPrx proxy, Ice.Current current)
    {
        System.out.println("initiating callback");
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
        System.out.println("Shutting down...");
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
