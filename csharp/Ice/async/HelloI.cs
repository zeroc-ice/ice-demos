// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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

    public override void sayHelloAsync(int delay, Action response, Action<Exception> exception, Ice.Current current)
    {
       if(delay == 0)
       {
           Console.Out.WriteLine("Hello World!");
           response();
       }
       else
       {
           _workQueue.Add(new WorkQueue.CallbackEntry(response, exception, delay));
       }
    }

    public override void shutdown(Ice.Current current)
    {
        _workQueue.destroy();
        current.adapter.getCommunicator().shutdown();
    }

    private WorkQueue _workQueue;
}
