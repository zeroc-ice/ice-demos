// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server extends com.zeroc.Ice.Application
{
    class ShutdownHook extends Thread
    {
        @Override
        public void run()
        {
            communicator().shutdown();
        }
    }

    @Override
    public int run(String[] args)
    {
        setInterruptHook(new ShutdownHook());

        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapter("Calculator");
        adapter.add(new CalculatorI(), com.zeroc.Ice.Util.stringToIdentity("calculator"));
        adapter.activate();

        communicator().waitForShutdown();
        return 0;
    }

    public static void main(String[] args)
    {
        Server app = new Server();
        int status = app.main("Server", args, "config.server");

        System.exit(status);
    }
}
