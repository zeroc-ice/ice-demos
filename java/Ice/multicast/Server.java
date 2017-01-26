// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server extends com.zeroc.Ice.Application
{
    @Override
    public int run(String[] args)
    {
        args = communicator().getProperties().parseCommandLineOptions("Discover", args);

        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapter("Hello");
        com.zeroc.Ice.ObjectAdapter discoverAdapter = communicator().createObjectAdapter("Discover");

        com.zeroc.Ice.ObjectPrx hello = adapter.addWithUUID(new HelloI());
        discoverAdapter.add(new DiscoverI(hello), com.zeroc.Ice.Util.stringToIdentity("discover"));

        discoverAdapter.activate();        
        adapter.activate();
        
        communicator().waitForShutdown();
        return 0;
    }

    public static void main(String[] args)
    {
        Server app = new Server();
        int status = app.main("Server", args, "config.server");
        System.exit(status);
    }
}
