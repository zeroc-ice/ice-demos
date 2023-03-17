//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using System;
using System.Threading;
using System.Threading.Tasks;

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
                    Console.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    using (var cts = new CancellationTokenSource())
                    {
                        Console.CancelKeyPress += (sender, eventArgs) =>
                        {
                            eventArgs.Cancel = true;
                            cts.Cancel();
                        };

                        Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Hello");
                        adapter.add(new HelloI(cts), Ice.Util.stringToIdentity("hello"));
                        adapter.activate();

                        // cts is canceled by Ctrl+C or a shutdown request.
                        // In modern C#, you should call: await Task.Delay(-1, cts.Token)
                        cts.Token.WaitHandle.WaitOne();
                        communicator.shutdown();
                    }
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
