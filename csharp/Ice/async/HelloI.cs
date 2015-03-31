// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using Demo;

public class HelloI : HelloDisp_
{
    public HelloI(WorkQueue workQueue)
    {
        _workQueue = workQueue;
    }

    public override void sayHello_async(AMD_Hello_sayHello cb, int delay, Ice.Current current)
    {
       if(delay == 0)
       {
           Console.Out.WriteLine("Hello World!");
           cb.ice_response();
       }
       else
       {
           _workQueue.Add(cb, delay);
       }
    }

    public override void shutdown(Ice.Current current)
    {
        _workQueue.destroy();
        current.adapter.getCommunicator().shutdown();
    }

    private WorkQueue _workQueue;
}
