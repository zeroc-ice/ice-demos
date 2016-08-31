// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    public static void main(String[] args)
    {
        try
        {
            com.zeroc.Ice.Util.InitializeResult r = com.zeroc.Ice.Util.initialize(args);
            com.zeroc.Ice.ObjectAdapter adapter =
                r.communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
            adapter.add(new HelloI(), com.zeroc.Ice.Util.stringToIdentity("hello"));
            adapter.activate();
            r.communicator.waitForShutdown();
            r.communicator.destroy();
        }
        catch(com.zeroc.Ice.LocalException ex)
        {
            ex.printStackTrace();
            System.exit(1);
        }
    }
}
