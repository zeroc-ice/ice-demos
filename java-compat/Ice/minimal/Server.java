// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    static class ShutdownHook extends Thread
    {
        @Override
        public void
        run()
        {
            _communicator.destroy();
        }

        ShutdownHook(Ice.Communicator communicator)
        {
            _communicator = communicator;
        }

        private final Ice.Communicator _communicator;
    }

    public static void
    main(String[] args)
    {
        try(Ice.Communicator communicator = Ice.Util.initialize(args))
        {
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new ShutdownHook(communicator));

            Ice.ObjectAdapter adapter =
                communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
            adapter.add(new HelloI(), Ice.Util.stringToIdentity("hello"));
            adapter.activate();
            communicator.waitForShutdown();
        }
    }
}
