// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    static class ShutdownHook extends Thread
    {
        @Override
        public void
        run()
        {
            _workQueue._destroy();
            try
            {
                _workQueue.join();
            }
            catch(java.lang.InterruptedException ex)
            {
            }
            finally
            {
                _communicator.destroy();
            }
        }

        ShutdownHook(Ice.Communicator communicator, WorkQueue workQueue)
        {
            _communicator = communicator;
            _workQueue = workQueue;
        }

        private final Ice.Communicator _communicator;
        private final WorkQueue _workQueue;
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
            // Install shutdown hook to destroy workqueue and communicator during JVM shutdown.
            //
            Runtime.getRuntime().addShutdownHook(new ShutdownHook(communicator, workQueue));

            if(argsHolder.value.length > 0)
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Hello");
                workQueue.start();

                adapter.add(new HelloI(workQueue), Ice.Util.stringToIdentity("hello"));
                adapter.activate();
                communicator.waitForShutdown();

                workQueue._destroy();
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
