// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Threading;
using ZeroC.Ice;

await using var communicator = new Communicator(ref args);
await communicator.ActivateAsync();

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) =>
    {
        eventArgs.Cancel = true;
        communicator.ShutdownAsync();
    };

var adapter = communicator.CreateObjectAdapterWithEndpoints("Hello", "ice+tcp://localhost:10000");
adapter.Add("hello", new Hello());
adapter.Activate();
await communicator.WaitForShutdownAsync();

public class Hello : IHello
{
    public void SayHello(Current current, CancellationToken cancel) => Console.WriteLine("Hello World!");
}
