// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class HelloI implements Hello
{
    public HelloI(WorkQueue workQueue)
    {
        _workQueue = workQueue;
    }

    @Override
    public java.util.concurrent.CompletionStage<Void> sayHelloAsync(int delay, com.zeroc.Ice.Current current)
    {
        if(delay == 0)
        {
            System.out.println("Hello World!");
            return java.util.concurrent.CompletableFuture.completedFuture((Void)null);
        }
        else
        {
            java.util.concurrent.CompletableFuture<Void> r = new java.util.concurrent.CompletableFuture<>();
            _workQueue.add(r, delay);
            return r;
        }
    }

    @Override
    public void shutdown(com.zeroc.Ice.Current current)
    {
        System.out.println("Shutting down...");

        _workQueue._destroy();
        current.adapter.getCommunicator().shutdown();
    }

    private WorkQueue _workQueue;
}
