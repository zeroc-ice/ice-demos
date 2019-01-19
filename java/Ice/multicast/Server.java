//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

public class Server
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<String>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.server", extraArgs))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.multicast");
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() ->
            {
                communicator.destroy();
                System.err.println("terminating");
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
