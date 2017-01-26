// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

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

        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapter("SessionFactory");

        java.util.concurrent.ScheduledExecutorService executor =
            java.util.concurrent.Executors.newScheduledThreadPool(1);
        ReapTask reaper = new ReapTask();
        executor.scheduleAtFixedRate(reaper, 1, 1, java.util.concurrent.TimeUnit.SECONDS);

        adapter.add(new SessionFactoryI(reaper), com.zeroc.Ice.Util.stringToIdentity("SessionFactory"));
        adapter.activate();
        communicator().waitForShutdown();

        executor.shutdown();
        reaper.terminate();

        return 0;
    }

    public static void main(String[] args)
    {
        Server app = new Server();
        int status = app.main("Server", args, "config.server");
        System.exit(status);
    }
}
