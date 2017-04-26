// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
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

        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapter("Hello");
        _workQueue = new WorkQueue();
        adapter.add(new HelloI(_workQueue), com.zeroc.Ice.Util.stringToIdentity("hello"));
        _workQueue.start();
        setInterruptHook(() ->
                         {
                             _workQueue._destroy();
                             communicator().shutdown();
                         });
        
        adapter.activate();

        communicator().waitForShutdown();

        try
        {
            _workQueue.join();
        }
        catch(java.lang.InterruptedException ex)
        {
        }

        return 0;
    }

    public static void main(String[] args)
    {
        Server app = new Server();
        int status = app.main("Server", args, "config.server");
        System.exit(status);
    }

    private WorkQueue _workQueue;
}
