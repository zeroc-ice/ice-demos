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
            // The new communicator is automatically destroyed (disposed) at the end of the
            // using statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.client"))
            {
                //
                // The communicator initialization removes all Ice-related arguments from args
                //
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
        //
        // Get the hello proxy. We configure the proxy to not cache the
        // server connection with the proxy and to disable the locator
        // cache. With this configuration, the IceGrid locator will be
        // queried for each invocation on the proxy and the invocation
        // will be sent over the server connection matching the returned
        // endpoints.
        //
        var obj = communicator.stringToProxy("hello");
        obj = obj.ice_connectionCached(false);
        obj = obj.ice_locatorCacheTimeout(0);

        var hello = HelloPrxHelper.checkedCast(obj);
        if(hello == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }

        while(true)
        {
            Console.Out.Write("enter the number of iterations: ");
            Console.Out.Flush();
            string line = Console.In.ReadLine();
            if(line == null || line.Equals("x"))
            {
                break;
            }
            int count = Convert.ToInt32(line);

            Console.Out.Write("enter the delay between each greetings (in ms): ");
            Console.Out.Flush();
            line = Console.In.ReadLine();
            if(line == null || line.Equals("x"))
            {
                break;
            }
            int delay = Convert.ToInt32(line);

            if(delay < 0)
            {
                delay = 500; // 500 milli-seconds
            }

            for(int i = 0; i < count; i++)
            {
                Console.Out.WriteLine(hello.getGreeting());
                System.Threading.Thread.Sleep(delay);
            }
        }

        return 0;
    }
}
