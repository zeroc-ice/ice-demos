//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Glacier2.callback.Demo.*;

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
        current.adapter.getCommunicator().shutdown();
    }
}
