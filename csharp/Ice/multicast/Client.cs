// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Reflection;

public class Client
{
    public class App : Ice.Application
    {
        public override int run(string[] args)
        {
            var adapter = communicator().createObjectAdapter("DiscoverReply");
            var replyI = new DiscoverReplyI();
            var reply = DiscoverReplyPrxHelper.uncheckedCast(adapter.addWithUUID(replyI));
            adapter.activate();

            var discover = DiscoverPrxHelper.uncheckedCast(
                communicator().propertyToProxy("Discover.Proxy").ice_datagram());
            discover.lookup(reply);
            var obj = replyI.waitReply(2000);
            if(obj == null)
            {
                Console.Error.WriteLine(appName() + ": no replies");
                return 1;
            }
            var hello = HelloPrxHelper.checkedCast(obj);
            if(hello == null)
            {
                Console.Error.WriteLine(appName() + ": invalid reply");
                return 1;
            }

            hello.sayHello();
            return 0;
        }
    }

    public static int Main(string[] args)
    {
        var app = new App();
        return app.main(args, "config.client");
    }
}
