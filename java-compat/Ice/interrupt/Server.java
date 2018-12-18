// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Server
{
    static class ShutdownHook extends Thread
    {
        @Override
        public void
        run()
        {
            //
            // Call shutdownNow on the executor. This interrupts all
            // executor threads causing any running servant dispatch threads
            // to terminate quickly.
            //
            _executor.shutdownNow();

            _communicator.destroy();
        }

        ShutdownHook(Ice.Communicator communicator, ExecutorService executor)
        {
            _communicator = communicator;
            _executor = executor;
        }

        private final Ice.Communicator _communicator;
        private final ExecutorService _executor;
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
        initData.dispatcher = new Ice.Dispatcher()
        {
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
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new ShutdownHook(communicator, executor));

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
