// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class HelloI extends _HelloDisp
{
    @Override
    public void
    sayHello(Ice.Current current)
    {
        current.adapter.getCommunicator().getLogger().print("Hello World!");
    }

    @Override
    public void
    shutdown(Ice.Current current)
    {
        current.adapter.getCommunicator().getLogger().print("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
