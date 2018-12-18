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
        HelloPrx hello = null;
        try
        {
            hello = HelloPrxHelper.checkedCast(communicator.stringToProxy("hello"));
        }
        catch(Ice.NotRegisteredException)
        {
            var query =
                IceGrid.QueryPrxHelper.checkedCast(communicator.stringToProxy("DemoIceGrid/Query"));
            hello = HelloPrxHelper.checkedCast(query.findObjectByType("::Demo::Hello"));
        }
        if(hello == null)
        {
            Console.WriteLine("couldn't find a `::Demo::Hello' object");
            return 1;
        }

        menu();

        string line = null;
        do
        {
            try
            {
                Console.Write("==> ");
                Console.Out.Flush();
                line = Console.In.ReadLine();
                if(line == null)
                {
                    break;
                }
                if(line.Equals("t"))
                {
                    hello.sayHello();
                }
                else if(line.Equals("s"))
                {
                    hello.shutdown();
                }
                else if(line.Equals("x"))
                {
                    // Nothing to do
                }
                else if(line.Equals("?"))
                {
                    menu();
                }
                else
                {
                    Console.WriteLine("unknown command `" + line + "'");
                    menu();
                }
            }
            catch(Ice.LocalException ex)
            {
                Console.WriteLine(ex);
            }
        }
        while(!line.Equals("x"));

        return 0;
    }

    private static void menu()
    {
        Console.WriteLine(
            "usage:\n" +
            "t: send greeting\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
