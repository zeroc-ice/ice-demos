// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    public static void main(String[] args) throws Exception
    {
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args))
        {
            //
            // Install shutdown hook for user interrupt like Ctrl-C
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() ->
            {
                //
                // Initiate communicator shutdown, waitForShutdown returns when complete
                // calling shutdown on a destroyed communicator is no-op
                //
                communicator.shutdown();
            }));

            com.zeroc.Ice.ObjectAdapter adapter =
                communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
            adapter.add(new HelloI(), com.zeroc.Ice.Util.stringToIdentity("hello"));
            adapter.activate();

            communicator.waitForShutdown();
        }
    }
}
