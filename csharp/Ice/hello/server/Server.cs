// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

try
{
    // using statement - communicator is automatically destroyed at the end of this statement

    await using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);
    await communicator.ActivateAsync();

    // Destroy the communicator on Ctrl+C or Ctrl+Break
    Console.CancelKeyPress += async (sender, eventArgs) => await communicator.ShutdownAsync();

    ObjectAdapter adapter = communicator.CreateObjectAdapter("Hello");
    adapter.Add("hello", new Hello());
    adapter.Activate();
    await communicator.WaitForShutdownAsync();
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}

return 0;
