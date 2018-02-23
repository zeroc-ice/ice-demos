// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import FilesystemI.*;

class Server
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<String>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, extraArgs))
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
                System.err.println("received signal, shutting down");
            }));

            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                //
                // Create an object adapter
                //
                com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints(
                    "LifecycleFilesystem", "default -h localhost -p 10000");

                //
                // Create the root directory.
                //
                DirectoryI root = new DirectoryI();
                com.zeroc.Ice.Identity id = new com.zeroc.Ice.Identity();
                id.name = "RootDir";
                adapter.add(root, id);

                //
                // All objects are created, allow client requests now.
                //
                adapter.activate();

                //
                // Wait until we are done.
                //
                communicator.waitForShutdown();
            }
        }

        System.exit(status);
    }
}
