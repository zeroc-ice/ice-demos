//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using System;

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
                    Console.CancelKeyPress += (sender, eventArgs) =>
                    {
                        eventArgs.Cancel = true;
                        communicator.shutdown();
                    };

                    Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Calculator");
                    adapter.add(new CalculatorI(), Ice.Util.stringToIdentity("calculator"));
                    adapter.activate();

                    communicator.waitForShutdown();
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
