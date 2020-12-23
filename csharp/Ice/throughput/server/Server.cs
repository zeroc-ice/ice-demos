// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Configuration;
using ZeroC.Ice;

try
{
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
        Console.Error.WriteLine("too many arguments");
        return 1;
    }

    ObjectAdapter adapter = communicator.CreateObjectAdapter("Throughput");
    adapter.Add("throughput", new Demo.Throughput());
    await adapter.ActivateAsync();
    await communicator.WaitForShutdownAsync();
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}

return 0;
