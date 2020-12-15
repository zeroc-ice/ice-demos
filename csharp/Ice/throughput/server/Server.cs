// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Configuration;
using ZeroC.Ice;

try
{
    // using statement - communicator is automatically destroyed at the end of this statement
    await using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);
    await communicator.ActivateAsync();

    // Destroy the communicator on Ctrl+C or Ctrl+Break
    Console.CancelKeyPress += async (sender, eventArgs) => await communicator.DisposeAsync();

    if (args.Length > 0)
    {
        Console.Error.WriteLine("too many arguments");
        return 1;
    }

    ObjectAdapter adapter = communicator.CreateObjectAdapter("Throughput");
    adapter.Add("throughput", new Demo.Throughput());
    adapter.Activate();
    await communicator.WaitForShutdownAsync();
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}

return 0;
