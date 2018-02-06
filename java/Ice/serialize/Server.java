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
        java.util.List<String> extraArgs = new java.util.ArrayList<String>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.server", extraArgs))
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

            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Greet");
                adapter.add(new GreetI(), com.zeroc.Ice.Util.stringToIdentity("greet"));
                adapter.activate();

                communicator.waitForShutdown();
            }
        }

        System.exit(status);
    }
}
