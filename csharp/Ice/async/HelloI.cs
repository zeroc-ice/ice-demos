// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Threading.Tasks;
using Demo;

public class HelloI : HelloDisp_
{
    public HelloI(WorkQueue workQueue)
    {
        _workQueue = workQueue;
    }

    public override Task sayHelloAsync(int delay, Ice.Current current)
    {
       if(delay == 0)
       {
           Console.Out.WriteLine("Hello World!");
           return null;
       }
       else
       {
           return _workQueue.Add(delay);
       }
    }

    public override void shutdown(Ice.Current current)
    {
        _workQueue.destroy();
        current.adapter.getCommunicator().shutdown();
    }

    private WorkQueue _workQueue;
}
