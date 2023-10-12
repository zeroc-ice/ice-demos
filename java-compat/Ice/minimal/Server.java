//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

public class Server
{
    static class ShutdownHook extends Thread
    {
        @Override
        public void
        run()
        {
            _communicator.shutdown();
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
            // Shut down the communicator on Ctrl+C
            Runtime.getRuntime().addShutdownHook(new ShutdownHook(communicator));

            Ice.ObjectAdapter adapter =
                communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
            adapter.add(new Printer(), Ice.Util.stringToIdentity("hello"));
            adapter.activate();
            communicator.waitForShutdown();
        }
    }
}
