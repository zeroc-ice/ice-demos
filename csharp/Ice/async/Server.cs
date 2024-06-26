// Copyright (c) ZeroC, Inc.

internal class Server
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            using Ice.Communicator communicator = Ice.Util.initialize(ref args, "config.server");
            if (args.Length > 0)
            {
                Console.WriteLine("too many arguments");
                status = 1;
            }
            else
            {
                using var cts = new CancellationTokenSource();
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

                // Wait for all dispatches to complete before disposing cts since the dispatches use cts or
                // cts.Token
                communicator.shutdown();
                communicator.waitForShutdown();
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
