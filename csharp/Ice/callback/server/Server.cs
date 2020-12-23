// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

await using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);
await communicator.ActivateAsync();

// Destroy the communicator on Ctrl+C or Ctrl+Break

Console.CancelKeyPress += async (sender, eventArgs) => await communicator.ShutdownAsync();

if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

var adapter = communicator.CreateObjectAdapter("Callback.Server");
adapter.Add("callbackSender", new CallbackSender());
await adapter.ActivateAsync();
await communicator.WaitForShutdownAsync();
