import Ice.minimal.HelloI;

// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<>();

        //
        // try with resource block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.server", extraArgs))
        {
            Runtime.getRuntime().addShutdownHook(new Thread(() ->
            {
                communicator.shutdown();
            }));

            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Hello");
                com.zeroc.Ice.ObjectAdapter discoverAdapter = communicator.createObjectAdapter("Discover");

                com.zeroc.Ice.ObjectPrx hello = adapter.addWithUUID(new HelloI());
                discoverAdapter.add(new DiscoverI(hello), com.zeroc.Ice.Util.stringToIdentity("discover"));

                discoverAdapter.activate();
                adapter.activate();

                communicator.waitForShutdown();
            }
        }

        System.exit(status);
    }
}
