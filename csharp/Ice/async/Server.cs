//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using System;
using System.Threading;

public class Server
{
    public static int Main(string[] args)
    {
        if (args.Length > 0)
        {
            Console.WriteLine("too many arguments");
            return 1;
        }

        try
        {
            using (var cts = new CancellationTokenSource())
            {
                using (Ice.Communicator communicator = Ice.Util.initialize(ref args, "config.server"))
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
                    // With C# 7.1 and up, you should make Main async and call: await Task.Delay(-1, cts.Token)
                    cts.Token.WaitHandle.WaitOne();
                }
            }
        }
        catch (Exception exception)
        {
            Console.WriteLine(exception);
            return 1;
        }

        return 0;
    }
}
