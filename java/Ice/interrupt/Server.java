// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Server extends com.zeroc.Ice.Application
{
    @Override
    public int run(String[] args)
    {
        if(args.length > 0)
        {
            System.err.println(appName() + ": too many arguments");
            return 1;
        }
        
        //
        // If ^C is pressed we want to interrupt all running upcalls from the
        // dispatcher and destroy the communicator.
        //
        setInterruptHook(new Thread()
        {
            @Override
            public void run()
            {
                //
                // Call shutdownNow on the executor. This interrupts all
                // executor threads causing any running servant dispatch threads
                // to terminate quickly.
                //
                _executor.shutdownNow();
                try
                {
                    communicator().shutdown();
                }
                catch(com.zeroc.Ice.LocalException ex)
                {
                    ex.printStackTrace();
                }
            }
        });

        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapter("TaskManager");
        adapter.add(new TaskManagerI(_executor), com.zeroc.Ice.Util.stringToIdentity("manager"));
        adapter.activate();
        communicator().waitForShutdown();
        
        return 0;
    }

    public static void main(String[] args)
    {
        final Server app = new Server();

        com.zeroc.Ice.InitializationData initData = new com.zeroc.Ice.InitializationData();
        initData.properties = com.zeroc.Ice.Util.createProperties();
        initData.properties.load("config.server");

        //
        // This demo uses a dispatcher to execute any invocations on the server.
        // By using an executor it is straightforward to interrupt any servant
        // dispatch threads by using ExecutorService.shutdownNow.
        //
        initData.dispatcher = (runnable, con) ->
        {
            app.getExecutor().submit(runnable);
        };
        
        int status = app.main("Server", args, initData);
        System.exit(status);
    }
    
    ExecutorService getExecutor()
    {
        return _executor;
    }

    private ExecutorService _executor = Executors.newFixedThreadPool(5);
}
