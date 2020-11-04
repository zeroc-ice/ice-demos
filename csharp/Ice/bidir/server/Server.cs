// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using System.Threading;
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

    // Shutdown the communicator on Ctrl+C or Ctrl+Break (shutdown always with Cancel = true)
    Console.CancelKeyPress += (eventSender, eventArgs) =>
        {
            Console.Write("disponse communicator...");
            eventArgs.Cancel = true;
            communicator.DisposeAsync();
            Console.WriteLine("ok");
        };

    ObjectAdapter adapter = communicator.CreateObjectAdapter("Callback.Server");
    var sender = new CallbackSender();
    adapter.Add("sender", sender);
    adapter.Activate();

    var t = new Thread(new ThreadStart(sender.Run));
    t.Start();

    try
    {
        communicator.WaitForShutdown();
    }
    finally
    {
        Console.WriteLine("Destroy sender");
        sender.Destroy();
        t.Join();
        Console.WriteLine("Destroy joined");
    }
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}
return 0;
