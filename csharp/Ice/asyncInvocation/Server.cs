//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using System;
using System.Threading;

public class Server
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            using (Ice.Communicator communicator = Ice.Util.initialize(ref args, "config.server"))
            {
                if (args.Length > 0)
                {
                    Console.Error.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    var shutdownRequested = new ManualResetEventSlim();
                    Console.CancelKeyPress += (sender, eventArgs) =>
                    {
                        eventArgs.Cancel = true;
                        shutdownRequested.Set();
                    };

                    Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Calculator");
                    adapter.add(new CalculatorI(shutdownRequested), Ice.Util.stringToIdentity("calculator"));
                    adapter.activate();

                    // Wait until shutdown is requested by Ctrl+C or a shutdown request.
                    shutdownRequested.Wait();
                    communicator.shutdown();
                }
            }
        }
        catch (Exception exception)
        {
            Console.WriteLine(exception);
            status = 1;
        }

        return status;
    }
}
