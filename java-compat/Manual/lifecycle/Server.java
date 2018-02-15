// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import FilesystemI.*;

class Server
{
    static class ShutdownHook implements Runnable
    {
        private Ice.Communicator communicator;

        ShutdownHook(Ice.Communicator communicator)
        {
            this.communicator = communicator;
        }

        @Override
        public void
        run()
        {
            //
            // Initiate communicator shutdown, waitForShutdown returns when complete
            // calling shutdown on a destroyed communicator is no-op
            //
            communicator.shutdown();
            System.err.println("received signal, shutting down");
        }
    }

    public static void
    main(String[] args)
    {
        int status = 0;
        Ice.StringSeqHolder argsHolder = new Ice.StringSeqHolder(args);

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder))
        {
            //
            // Install shutdown hook for user interrupt like Ctrl-C
            //
            Runtime.getRuntime().addShutdownHook(new Thread(new ShutdownHook(communicator)));

            if(argsHolder.value.length > 0)
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                //
                // Create an object adapter
                //
                Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints(
                    "LifecycleFilesystem", "default -h localhost -p 10000");

                //
                // Create the root directory.
                //
                DirectoryI root = new DirectoryI();
                Ice.Identity id = new Ice.Identity();
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
