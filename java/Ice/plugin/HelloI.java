// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class HelloI implements Hello
{
    @Override
    public void sayHello(com.zeroc.Ice.Current current)
    {
        current.adapter.getCommunicator().getLogger().print("Hello World!");
    }

    @Override
    public void shutdown(com.zeroc.Ice.Current current)
    {
        current.adapter.getCommunicator().getLogger().print("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
