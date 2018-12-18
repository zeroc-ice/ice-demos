// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;

public class Client
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            //
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.client"))
            {
                //
                // Destroy the communicator on Ctrl+C or Ctrl+Break
                //
                Console.CancelKeyPress += (sender, eventArgs) => communicator.destroy();

                if(args.Length > 0)
                {
                    Console.Error.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    status = run(communicator);
                }
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int run(Ice.Communicator communicator)
    {
        var adapter = communicator.createObjectAdapter("DiscoverReply");
        var replyI = new DiscoverReplyI();
        var reply = DiscoverReplyPrxHelper.uncheckedCast(adapter.addWithUUID(replyI));
        adapter.activate();

        var discover = DiscoverPrxHelper.uncheckedCast(
            communicator.propertyToProxy("Discover.Proxy").ice_datagram());
        discover.lookup(reply);
        var obj = replyI.waitReply(2000);
        if(obj == null)
        {
            Console.Error.WriteLine("no replies");
            return 1;
        }
        var hello = HelloPrxHelper.checkedCast(obj);
        if(hello == null)
        {
            Console.Error.WriteLine("invalid reply");
            return 1;
        }

        hello.sayHello();
        return 0;
    }
}
