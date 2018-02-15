// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    static class ShutdownHook implements Runnable
    {
        private Ice.Communicator communicator;
        private WorkQueue workQueue;

        ShutdownHook(Ice.Communicator communicator, WorkQueue workQueue)
        {
            this.communicator = communicator;
            this.workQueue = workQueue;
        }

        @Override
        public void
        run()
        {
            workQueue._destroy();
            //
            // Initiate communicator shutdown, waitForShutdown returns when complete
            // calling shutdown on a destroyed communicator is no-op
            //
            communicator.shutdown();
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
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, "config.server"))
        {
            WorkQueue workQueue = new WorkQueue();

            //
            // Install shutdown hook for user interrupt like Ctrl-C
            //
            Runtime.getRuntime().addShutdownHook(new Thread(new ShutdownHook(communicator, workQueue)));

            if(argsHolder.value.length > 0)
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Hello");
                adapter.add(new HelloI(workQueue), Ice.Util.stringToIdentity("hello"));
                workQueue.start();
                adapter.activate();

                communicator.waitForShutdown();

                try
                {
                    workQueue.join();
                }
                catch(java.lang.InterruptedException ex)
                {
                }
            }
        }

        System.exit(status);
    }
}
