// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Threading;
using System.Threading.Tasks;
using ZeroC.Ice;

public class Hello : IHello
{
    private readonly WorkQueue _workQueue;

    public Hello(WorkQueue workQueue) => _workQueue = workQueue;

    public ValueTask SayHelloAsync(int delay, Current current, CancellationToken cancel)
    {
        if (delay == 0)
        {
            Console.WriteLine("Hello World!");
            return default;
        }
        else
        {
            return new ValueTask(_workQueue.Add(delay));
        }
    }

    public void Shutdown(Current current, CancellationToken cancel)
    {
        _workQueue.Destroy();
        current.Adapter.Communicator.ShutdownAsync();
    }
}
