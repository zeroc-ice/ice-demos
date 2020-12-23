// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Configuration;
using System.IO;
using ZeroC.Ice;

Console.WriteLine($"The current directory is {Directory.GetCurrentDirectory()}");

await using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);
await communicator.ActivateAsync();

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) =>
    {
        eventArgs.Cancel = true;
        _ = communicator.ShutdownAsync();
    };

if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

await communicator.WaitForShutdownAsync();
