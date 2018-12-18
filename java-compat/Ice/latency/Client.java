// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class Client
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
        int status = 0;
        Ice.StringSeqHolder argsHolder = new Ice.StringSeqHolder(args);

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, "config.client"))
        {
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new ShutdownHook(communicator));

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

    private static int
    run(Ice.Communicator communicator)
    {
        PingPrx ping = PingPrxHelper.checkedCast(communicator.propertyToProxy("Ping.Proxy"));
        if(ping == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        //
        // A method needs to be invoked thousands of times before the JIT compiler
        // will convert it to native code. To ensure an accurate latency measurement,
        // we need to "warm up" the JIT compiler.
        //
        {
            final int repetitions = 20000;
            System.out.print("warming up the JIT compiler...");
            System.out.flush();
            for(int i = 0; i < repetitions; i++)
            {
                ping.ice_ping();
            }
            System.out.println(" ok");
        }

        long tv1 = System.currentTimeMillis();
        final int repetitions = 100000;
        System.out.println("pinging server " + repetitions + " times (this may take a while)");
        for(int i = 0; i < repetitions; i++)
        {
            ping.ice_ping();
        }

        long tv2 = System.currentTimeMillis();
        double total = tv2 - tv1;
        double perPing = total / repetitions;

        System.out.println("time for " + repetitions + " pings: " + total + "ms");
        System.out.println("time per ping: " + perPing + "ms");

        return 0;
    }
}
