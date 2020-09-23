// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

try
{
    // using statement - communicator is automatically destroyed at the end of this statement
    using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

    if (args.Length > 0)
    {
        Console.Error.WriteLine("too many arguments");
        return 1;
    }

    var workQueue = new WorkQueue();

    // Shutdown the communicator and destroy the workqueue on Ctrl+C or Ctrl+Break
    // (shutdown always with Cancel = true)
    Console.CancelKeyPress += (sender, eventArgs) =>
    {
        eventArgs.Cancel = true;
        workQueue.Destroy();
        communicator.Dispose();
    };

    ObjectAdapter adapter = communicator.CreateObjectAdapter("Hello");
    adapter.Add("hello", new Hello(workQueue));

    workQueue.Start();

    try
    {
        adapter.Activate();
        communicator.WaitForShutdown();
    }
    finally
    {
        workQueue.Join();
    }
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}
return 0;
