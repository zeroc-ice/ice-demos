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
        try
        {
            using(var communicator = Ice.Util.initialize(ref args))
            {
                //
                // Destroy the communicator on Ctrl+C or Ctrl+Break
                //
                Console.CancelKeyPress += (sender, eventArgs) => communicator.destroy();
                var adapter = communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
                adapter.add(new HelloI(), Ice.Util.stringToIdentity("hello"));
                adapter.activate();
                communicator.waitForShutdown();
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            return 1;
        }
        return 0;
    }
}
