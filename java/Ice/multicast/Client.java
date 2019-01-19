//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.multicast.Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.client", extraArgs))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.multicast");
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() -> communicator.destroy()));

            if(!extraArgs.isEmpty())
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

    private static int run(com.zeroc.Ice.Communicator communicator)
    {
        com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapter("DiscoverReply");
        DiscoverReplyI replyI = new DiscoverReplyI();
        DiscoverReplyPrx reply = DiscoverReplyPrx.uncheckedCast(adapter.addWithUUID(replyI));
        adapter.activate();

        DiscoverPrx discover = DiscoverPrx.uncheckedCast(
            communicator.propertyToProxy("Discover.Proxy")).ice_datagram();
        discover.lookup(reply);
        com.zeroc.Ice.ObjectPrx base = replyI.waitReply(2000);

        if(base == null)
        {
            System.err.println("no replies");
            return 1;
        }
        HelloPrx hello = HelloPrx.checkedCast(base);
        if(hello == null)
        {
            System.err.println("invalid reply");
            return 1;
        }

        hello.sayHello();
        return 0;
    }
}
