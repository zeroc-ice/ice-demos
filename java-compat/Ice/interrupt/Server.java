// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Server
{
    static class ShutdownHook implements Runnable
    {
        private Ice.Communicator communicator;
        private ExecutorService executor;

        ShutdownHook(Ice.Communicator communicator, ExecutorService executor)
        {
            this.communicator = communicator;
            this.executor = executor;
        }

        @Override
        public void
        run()
        {
            //
            // Call shutdownNow on the executor. This interrupts all
            // executor threads causing any running servant dispatch threads
            // to terminate quickly.
            //
            executor.shutdownNow();

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

        final ExecutorService executor = Executors.newFixedThreadPool(5);

        Ice.InitializationData initData = new Ice.InitializationData();
        initData.properties = Ice.Util.createProperties();
        initData.properties.load("config.server");

        //
        // This demo uses a dispatcher to execute any invocations on the server.
        // By using an executor it is straightforward to interrupt any servant
        // dispatch threads by using ExecutorService.shutdownNow.
        //
        initData.dispatcher = new Ice.Dispatcher() {
            @Override
            public void dispatch(Runnable runnable, Ice.Connection con)
            {
                executor.submit(runnable);
            }
        };

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, initData))
        {
            //
            // If ^C is pressed we want to interrupt all running upcalls from the
            // dispatcher and destroy the communicator.
            //
            Runtime.getRuntime().addShutdownHook(new Thread(new ShutdownHook(communicator, executor)));

            if(argsHolder.value.length > 0)
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                Ice.ObjectAdapter adapter = communicator.createObjectAdapter("TaskManager");
                adapter.add(new TaskManagerI(executor), Ice.Util.stringToIdentity("manager"));
                adapter.activate();

                communicator.waitForShutdown();
            }
        }

        System.exit(status);
    }
}
