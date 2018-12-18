// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Threading;

public class Server
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            //
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.server"))
            {
                if(args.Length > 0)
                {
                    Console.Error.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    //
                    // Shutdown the communicator on Ctrl+C or Ctrl+Break
                    // (shutdown always with Cancel = true)
                    //
                    Console.CancelKeyPress += (eventSender, eventArgs) =>
                    {
                        eventArgs.Cancel = true;
                        communicator.shutdown();
                    };

                    var adapter = communicator.createObjectAdapter("Callback.Server");
                    var sender = new CallbackSenderI(communicator);
                    adapter.add(sender, Ice.Util.stringToIdentity("sender"));
                    adapter.activate();

                    var t = new Thread(new ThreadStart(sender.Run));
                    t.Start();

                    try
                    {
                        communicator.waitForShutdown();
                    }
                    finally
                    {
                        sender.destroy();
                        t.Join();
                    }
                }
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }
}
