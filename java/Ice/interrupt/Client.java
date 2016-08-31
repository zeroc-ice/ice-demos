// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import Demo.*;
import com.zeroc.Ice.LocalException;

public class Client extends com.zeroc.Ice.Application
{
    class ShutdownHook extends Thread
    {
        @Override
        public void run()
        {
            try
            {
                communicator().destroy();
            }
            catch(com.zeroc.Ice.LocalException ex)
            {
                ex.printStackTrace();
            }
        }
    }

    private static void
    menu()
    {
        System.out.println(
            "usage:\n" +
            "t: start a task\n" +
            "b: start a blocking task\n" +
            "i: interrupt the blocking task\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }

    @Override
    public int run(String[] args)
    {
        if(args.length > 0)
        {
            System.err.println(appName() + ": too many arguments");
            return 1;
        }

        //
        // Since this is an interactive demo we want to clear the
        // Application installed interrupt callback and install our
        // own shutdown hook.
        //
        setInterruptHook(new ShutdownHook());

        final TaskManagerPrx taskManager =
            TaskManagerPrx.checkedCast(communicator().propertyToProxy("TaskManager.Proxy"));
        if(taskManager == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        menu();

        java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
        
        ExecutorService executor = Executors.newFixedThreadPool(5);
        List<Future<?> > futures = new ArrayList<>();
        int nextId = 0;
        String line = null;
        do
        {
            try
            {
                System.out.print("==> ");
                System.out.flush();
                line = in.readLine();
                if(line == null)
                {
                    break;
                }
                if(line.equals("t"))
                {
                    final int id = nextId++;
                    taskManager.runAsync(id).whenComplete((result, ex) ->
                    {
                        if(ex == null)
                        {
                            System.out.println("task " + id + " completed running");
                        }
                        else
                        {
                            System.out.println("blocking task " + id + " failed");
                            ex.printStackTrace();
                        }
                    });
                }
                else if(line.equals("b"))
                {
                    //
                    // Remove any completed tasks.
                    //
                    Iterator<Future<?> > iterator = futures.iterator();
                    while(iterator.hasNext())
                    {
                        Future<?> f = iterator.next();
                        if(f.isDone())
                        {
                            iterator.remove();
                        }
                    }

                    final int id = nextId++;
                    Future<?> future = executor.submit(() ->
                    {
                        try
                        {
                            taskManager.run(id);
                            System.out.println("task " + id + " completed running");
                        } 
                        catch(com.zeroc.Ice.OperationInterruptedException e)
                        {
                            System.out.println("blocking task " + id + " interrupted");
                        }
                        catch(com.zeroc.Ice.Exception e)
                        {
                            System.out.println("blocking task " + id + " failed");
                            e.printStackTrace();
                        }                                
                    });
                    futures.add(future);
                }
                else if(line.equals("i"))
                {
                    for(Future<?> f : futures)
                    {
                        f.cancel(true);
                    }
                    futures.clear();
                }
                else if(line.equals("s"))
                {
                    taskManager.shutdown();
                }
                else if(line.equals("x"))
                {
                    // Nothing to do
                }
                else if(line.equals("?"))
                {
                    menu();
                }
                else
                {
                    System.out.println("unknown command `" + line + "'");
                    menu();
                }
            }
            catch(java.io.IOException ex)
            {
                ex.printStackTrace();
            }
            catch(com.zeroc.Ice.LocalException ex)
            {
                ex.printStackTrace();
            }
        }
        while(!line.equals("x"));

        return 0;
    }

    public static void main(String[] args)
    {
        Client app = new Client();
        int status = app.main("Client", args, "config.client");
        System.exit(status);
    }
}

