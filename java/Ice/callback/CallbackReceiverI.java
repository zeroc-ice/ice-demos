// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public final class CallbackReceiverI implements CallbackReceiver
{
    @Override
    public void callback(com.zeroc.Ice.Current current)
    {
        System.out.println("received callback");
    }
}
