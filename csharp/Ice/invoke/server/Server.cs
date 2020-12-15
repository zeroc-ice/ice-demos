// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

try
{
    // using statement - communicator is automatically destroyed at the end of this statement
    await using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

    // Destroy the communicator on Ctrl+C or Ctrl+Break
    Console.CancelKeyPress += (sender, eventArgs) => communicator.ShutdownAsync();

    if (args.Length > 0)
    {
        throw new ArgumentException("too many arguments");
    }

    ObjectAdapter adapter = communicator.CreateObjectAdapter("Printer");
    adapter.Add("printer", new Printer());
    adapter.Activate();
    await communicator.WaitForShutdownAsync();
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}
return 0;
