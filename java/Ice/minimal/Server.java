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
            // Install shutdown hook to destroy communicator during JVM shutdown,
            // including when the user interrupts the application with Ctrl-C
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() ->
            {
                //
                // Destroy returns only when the communicator is fully destroyed
                //
                communicator.destroy();
            }));

            com.zeroc.Ice.ObjectAdapter adapter =
                communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
            adapter.add(new HelloI(), com.zeroc.Ice.Util.stringToIdentity("hello"));
            adapter.activate();

            communicator.waitForShutdown();
        }
    }
}
