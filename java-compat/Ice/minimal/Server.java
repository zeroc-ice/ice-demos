// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    public static void
    main(String[] args)
    {
        try
        {
            Ice.Communicator communicator = Ice.Util.initialize(args);
            Ice.ObjectAdapter adapter =
                communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
            adapter.add(new HelloI(), Ice.Util.stringToIdentity("hello"));
            adapter.activate();
            communicator.waitForShutdown();
            communicator.destroy();
        }
        catch(Ice.LocalException ex)
        {
            ex.printStackTrace();
            System.exit(1);
        }
    }
}
