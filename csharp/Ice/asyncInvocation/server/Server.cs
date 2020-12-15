// Copyright (c) ZeroC, Inc. All rights reserved.

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
        Console.Error.WriteLine("too many arguments");
        return 1;
    }

    ObjectAdapter adapter = communicator.CreateObjectAdapter("Calculator");
    adapter.Add("calculator", new Demo.Calculator());
    adapter.Activate();
    await communicator.WaitForShutdownAsync();
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}
return 0;
