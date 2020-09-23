// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using ZeroC.Ice;

using var communicator = new Communicator(ref args);

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) => communicator.DisposeAsync();

var adapter = communicator.CreateObjectAdapterWithEndpoints("Hello", "ice+tcp://localhost:10000");
adapter.Add("hello", new Hello());
adapter.Activate();
communicator.WaitForShutdown();

public class Hello : IHello
{
    public void SayHello(Current current) => Console.WriteLine("Hello World!");
}
