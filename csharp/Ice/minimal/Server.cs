// Copyright (c) ZeroC, Inc.

internal class Server
{
    public static int Main(string[] args)
    {
        try
        {
            using var communicator = Ice.Util.initialize(ref args);
            // Shut down the communicator on Ctrl+C or Ctrl+Break.
            Console.CancelKeyPress += (sender, eventArgs) =>
            {
                eventArgs.Cancel = true;
                communicator.shutdown();
            };

            var adapter = communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
            adapter.add(new Printer(), Ice.Util.stringToIdentity("hello"));
            adapter.activate();
            communicator.waitForShutdown();
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
            return 1;
        }
        return 0;
    }
}
