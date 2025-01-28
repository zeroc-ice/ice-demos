// Copyright (c) ZeroC, Inc.

import com.zeroc.demos.Glacier2.callback.Demo.*;

public final class CallbackReceiverI implements CallbackReceiver
{
    @Override
    public void callback(com.zeroc.Ice.Current current)
    {
        System.out.println("received callback");
    }
}
