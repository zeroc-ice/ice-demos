// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import Ice.hello.HelloI;

public class Server
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<>();

        com.zeroc.Ice.InitializationData initData = new com.zeroc.Ice.InitializationData();
        initData.properties = com.zeroc.Ice.Util.createProperties();
        initData.properties.load("config.server");

        //
        // This demo uses a dispatcher to execute any invocations on the server.
        // By using an executor it is straightforward to interrupt any servant
        // dispatch threads by using ExecutorService.shutdownNow.
        //
        ExecutorService executor = Executors.newFixedThreadPool(5);
        initData.dispatcher = (runnable, con) ->
        {
            executor.submit(runnable);
        };

        //
        // try with resource block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, initData, extraArgs))
        {
            //
            // Call shutdownNow on the executor. This interrupts all
            // executor threads causing any running servant dispatch threads
            // to terminate quickly.
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() ->
            {
                communicator.shutdown();
                executor.shutdownNow();
            }));

            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapter("TaskManager");
                adapter.add(new TaskManagerI(executor), com.zeroc.Ice.Util.stringToIdentity("manager"));
                adapter.activate();

                communicator.waitForShutdown();
            }
        }

        System.exit(status);
    }
}
