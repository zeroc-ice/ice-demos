// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client extends com.zeroc.Ice.Application
{
    @Override
    public int run(String[] args)
    {
        args = communicator().getProperties().parseCommandLineOptions("Discover", args);

        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapter("DiscoverReply");
        DiscoverReplyI replyI = new DiscoverReplyI();
        DiscoverReplyPrx reply = DiscoverReplyPrx.uncheckedCast(adapter.addWithUUID(replyI));
        adapter.activate();

        DiscoverPrx discover = DiscoverPrx.uncheckedCast(
            communicator().propertyToProxy("Discover.Proxy")).ice_datagram();
        discover.lookup(reply);
        com.zeroc.Ice.ObjectPrx base = replyI.waitReply(2000);

        if(base == null)
        {
            System.err.println(appName() + ": no replies");
            return 1;
        }
        HelloPrx hello = HelloPrx.checkedCast(base);
        if(hello == null)
        {
            System.err.println(appName() + ": invalid reply");
            return 1;
        }

        hello.sayHello();
        return 0;
    }

    public static void main(String[] args)
    {
        Client app = new Client();
        int status = app.main("Client", args, "config.client");
        System.exit(status);
    }
}
