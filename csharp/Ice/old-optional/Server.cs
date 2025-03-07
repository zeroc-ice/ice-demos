// Copyright (c) ZeroC, Inc.

internal class Server
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            using var communicator = Ice.Util.initialize(ref args, "config.server");
            // Destroy the communicator on Ctrl+C or Ctrl+Break
            Console.CancelKeyPress += (sender, eventArgs) => communicator.destroy();

            if (args.Length > 0)
            {
                Console.Error.WriteLine("too many arguments");
                status = 1;
            }
            else
            {
                var adapter = communicator.createObjectAdapter("ContactDB");
                adapter.add(new ContactDBI(), Ice.Util.stringToIdentity("contactdb"));
                adapter.activate();
                communicator.waitForShutdown();
            }
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }
}
