// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Configuration;
using ZeroC.Ice;

await using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);
await communicator.ActivateAsync();

// Initiates communicator shutdown on Ctrl+C or Ctrl+Break, but does not wait for shutdown to complete.
Console.CancelKeyPress += (sender, eventArgs) =>
    {
        eventArgs.Cancel = true;
        communicator.ShutdownAsync();
    };

if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

ObjectAdapter adapter = communicator.CreateObjectAdapter("Latency");
adapter.Add("ping", new Ping());
await adapter.ActivateAsync();
await communicator.WaitForShutdownAsync();

class Ping : Demo.IPing
{
}
