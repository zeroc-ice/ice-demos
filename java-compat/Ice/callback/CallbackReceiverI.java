// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public final class CallbackReceiverI extends _CallbackReceiverDisp
{
    @Override
    public void
    callback(Ice.Current current)
    {
        System.out.println("received callback");
    }
}
