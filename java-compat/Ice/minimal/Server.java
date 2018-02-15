// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    static class ShutdownHook implements Runnable
    {
        private Ice.Communicator communicator;

        ShutdownHook(Ice.Communicator communicator)
        {
            this.communicator = communicator;
        }

        @Override
        public void
        run()
        {
            //
            // Initiate communicator shutdown, waitForShutdown returns when complete
            // calling shutdown on a destroyed communicator is no-op
            //
            communicator.shutdown();
        }
    }

    public static void
    main(String[] args) throws Exception
    {
        try(Ice.Communicator communicator = Ice.Util.initialize(args))
        {
            //
            // Install shutdown hook for user interrupt like Ctrl-C
            //
            Runtime.getRuntime().addShutdownHook(new Thread(new ShutdownHook(communicator)));

            Ice.ObjectAdapter adapter =
                communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
            adapter.add(new HelloI(), Ice.Util.stringToIdentity("hello"));
            adapter.activate();
            communicator.waitForShutdown();
            communicator.destroy();
        }
    }
}
