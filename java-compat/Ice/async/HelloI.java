// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class HelloI extends _HelloDisp
{
    public 
    HelloI(WorkQueue workQueue)
    {
        _workQueue = workQueue;
    }

    @Override
    public void
    sayHello_async(AMD_Hello_sayHello cb, int delay, Ice.Current current)
    {
        if(delay == 0)
        {
            System.out.println("Hello World!");
            cb.ice_response();
        }
        else
        {
            _workQueue.add(cb, delay);
        }
    }

    @Override
    public void
    shutdown(Ice.Current current)
    {
        System.out.println("Shutting down...");

        _workQueue._destroy();
        current.adapter.getCommunicator().shutdown();
    }

    private WorkQueue _workQueue;
}
