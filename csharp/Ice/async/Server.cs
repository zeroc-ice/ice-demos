// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;

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
                    var workQueue = new WorkQueue();

                    //
                    // Shutdown the communicator and destroy the workqueue on Ctrl+C or Ctrl+Break
                    // (shutdown always with Cancel = true)
                    //
                    Console.CancelKeyPress += (sender, eventArgs) =>
                    {
                        eventArgs.Cancel = true;
                        workQueue.destroy();
                        communicator.shutdown();
                    };

                    var adapter = communicator.createObjectAdapter("Hello");
                    adapter.add(new HelloI(workQueue), Ice.Util.stringToIdentity("hello"));

                    workQueue.Start();
                    try
                    {
                        adapter.activate();
                        communicator.waitForShutdown();
                    }
                    finally
                    {
                        workQueue.Join();
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
