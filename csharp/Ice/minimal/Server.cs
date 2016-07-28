// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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
                var adapter = communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
                adapter.add(new HelloI(), Ice.Util.stringToIdentity("hello"));
                adapter.activate();
                communicator.waitForShutdown();
            }
            return 0;
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            return 1;
        }
    }
}
