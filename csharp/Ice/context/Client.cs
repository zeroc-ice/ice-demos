// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;

using System;
using System.Collections.Generic;

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
        var proxy = ContextPrxHelper.checkedCast(communicator.propertyToProxy("Context.Proxy"));
        if(proxy == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }

        menu();

        string line = null;
        do
        {
            try
            {
                Console.Out.Write("==> ");
                Console.Out.Flush();
                line = Console.In.ReadLine();
                if(line == null)
                {
                    break;
                }
                if(line.Equals("1"))
                {
                    proxy.call();
                }
                else if(line.Equals("2"))
                {
                    var ctx = new Dictionary<string, string>() { { "type", "Explicit" } };
                    proxy.call(ctx);
                }
                else if(line.Equals("3"))
                {
                    var ctx = new Dictionary<string, string>() { { "type", "Per-Proxy" } };
                    ContextPrxHelper.uncheckedCast(proxy.ice_context(ctx)).call();
                }
                else if(line.Equals("4"))
                {
                    var ic = communicator.getImplicitContext();
                    var ctx = new Dictionary<string, string>() { { "type", "Implicit" } };
                    ic.setContext(ctx);
                    proxy.call();
                    ctx = new Dictionary<string, string>();
                    ic.setContext(ctx);
                }
                else if(line.Equals("s"))
                {
                    proxy.shutdown();
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
            catch(Exception ex)
            {
                Console.Error.WriteLine(ex);
            }
        }
        while (!line.Equals("x"));

        return 0;
    }

    private static void menu()
    {
        Console.Write(
            "usage:\n" +
            "1: call with no request context\n" +
            "2: call with explicit request context\n" +
            "3: call with per-proxy request context\n" +
            "4: call with implicit request context\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
