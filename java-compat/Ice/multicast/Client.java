// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
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
        Ice.ObjectAdapter adapter = communicator.createObjectAdapter("DiscoverReply");
        DiscoverReplyI replyI = new DiscoverReplyI();
        DiscoverReplyPrx reply = DiscoverReplyPrxHelper.uncheckedCast(adapter.addWithUUID(replyI));
        adapter.activate();

        DiscoverPrx discover = DiscoverPrxHelper.uncheckedCast(
            communicator.propertyToProxy("Discover.Proxy").ice_datagram());
        discover.lookup(reply);
        Ice.ObjectPrx base = replyI.waitReply(2000);

        if(base == null)
        {
            System.err.println("no replies");
            return 1;
        }

        HelloPrx hello = HelloPrxHelper.checkedCast(base);
        if(hello == null)
        {
            System.err.println("invalid reply");
            return 1;
        }

        hello.sayHello();
        return 0;
    }
}
