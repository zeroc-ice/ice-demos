// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server extends Ice.Application
{
    @Override
    public int
    run(String[] args)
    {
        args = communicator().getProperties().parseCommandLineOptions("Discover", args);

        Ice.ObjectAdapter adapter = communicator().createObjectAdapter("Hello");
        Ice.ObjectAdapter discoverAdapter = communicator().createObjectAdapter("Discover");

        Ice.ObjectPrx hello = adapter.addWithUUID(new HelloI());
        discoverAdapter.add(new DiscoverI(hello), Ice.Util.stringToIdentity("discover"));

        discoverAdapter.activate();        
        adapter.activate();
        
        communicator().waitForShutdown();
        return 0;
    }

    public static void
    main(String[] args)
    {
        Server app = new Server();
        int status = app.main("Server", args, "config.server");
        System.exit(status);
    }
}
