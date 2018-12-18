// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import Demo.*;
import Ice.LocalException;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        Ice.StringSeqHolder argsHolder = new Ice.StringSeqHolder(args);

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, "config.client"))
        {
            if(argsHolder.value.length > 0)
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                status = run(communicator);
            }
        }

        System.exit(status);
    }

    private static int run(Ice.Communicator communicator)
    {
        final TaskManagerPrx taskManager = TaskManagerPrxHelper.checkedCast(communicator.propertyToProxy("TaskManager.Proxy"));
        if(taskManager == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        menu();

        java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));

        ExecutorService executor = Executors.newFixedThreadPool(5);
        List<Future<?> > futures = new ArrayList<Future<?> >();
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
                    taskManager.begin_run(id, new Callback_TaskManager_run()
                    {
                        @Override
                        public void response()
                        {
                            System.out.println("task " + id + " completed running");
                        }

                        @Override
                        public void exception(LocalException ex)
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
                    while(iterator.hasNext()) {
                        Future<?> f = iterator.next();
                        if(f.isDone()) {
                            iterator.remove();
                        }
                    }

                    final int id = nextId++;
                    Future<?> future = executor.submit(new Runnable() {
                        @Override
                        public void
                        run()
                        {
                            try
                            {
                                taskManager.run(id);
                                System.out.println("task " + id + " completed running");
                            }
                            catch(Ice.OperationInterruptedException e)
                            {
                                System.out.println("blocking task " + id + " interrupted");
                            }
                            catch(Ice.Exception e)
                            {
                                System.out.println("blocking task " + id + " failed");
                                e.printStackTrace();
                            }
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
            catch(Ice.LocalException ex)
            {
                ex.printStackTrace();
            }
        }
        while(!line.equals("x"));

        return 0;
    }

    private static void menu()
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
}
