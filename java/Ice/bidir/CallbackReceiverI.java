// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class CallbackReceiverI implements CallbackReceiver
{
    CallbackReceiverI()
    {
    }

    @Override
    public void callback(int num, com.zeroc.Ice.Current current)
    {
        System.out.println("received callback #" + num);
    }
}
