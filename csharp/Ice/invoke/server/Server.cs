// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

try
{
    await using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);
    await communicator.ActivateAsync();

    // Destroy the communicator on Ctrl+C or Ctrl+Break
    Console.CancelKeyPress += async (sender, eventArgs) => await communicator.ShutdownAsync();

    if (args.Length > 0)
    {
        throw new ArgumentException("too many arguments");
    }

    ObjectAdapter adapter = communicator.CreateObjectAdapter("Printer");
    adapter.Add("printer", new Printer());
    await adapter.ActivateAsync();
    await communicator.WaitForShutdownAsync();
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}
return 0;
