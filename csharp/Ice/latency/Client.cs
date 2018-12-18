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
        PingPrx ping = PingPrxHelper.checkedCast(communicator.propertyToProxy("Ping.Proxy"));
        if(ping == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }

        //
        // A method needs to be invoked thousands of times before the JIT compiler
        // will convert it to native code. To ensure an accurate latency measurement,
        // we need to "warm up" the JIT compiler.
        //
        {
            int reps = 20000;
            Console.Error.Write("warming up the JIT compiler...");
            Console.Error.Flush();
            for(int i = 0; i < reps; i++)
            {
                ping.ice_ping();
            }
            Console.Error.WriteLine("ok");
        }

        var tv1 = DateTime.Now;
        int repetitions = 100000;
        Console.Out.WriteLine("pinging server " + repetitions + " times (this may take a while)");
        for (int i = 0; i < repetitions; i++)
        {
            ping.ice_ping();
        }

        var total = (DateTime.Now - tv1).TotalMilliseconds;
        var perPing = total / repetitions;

        Console.Out.WriteLine("time for " + repetitions + " pings: " + total + "ms");
        Console.Out.WriteLine("time per ping: " + perPing + "ms");

        return 0;
    }
}
