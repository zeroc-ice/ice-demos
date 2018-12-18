// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Threading;
using System.Threading.Tasks;

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
        var hello = HelloPrxHelper.checkedCast(communicator.propertyToProxy("Hello.Proxy"));
        if(hello == null)
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
                if(line.Equals("i"))
                {
                    hello.sayHello(0);
                }
                else if(line.Equals("d"))
                {
                    helloAsync(hello);
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
                    Console.Out.WriteLine("unknown command `" + line + "'");
                    menu();
                }
            }
            catch(Ice.Exception ex)
            {
                Console.Error.WriteLine(ex);
            }
        }
        while(!line.Equals("x"));

        return 0;
    }

    private static async void helloAsync(HelloPrx hello)
    {
        try
        {
            await hello.sayHelloAsync(5000);
        }
        catch(RequestCanceledException)
        {
            Console.Error.WriteLine("RequestCanceledException");
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine("sayHello AMI call failed:");
            Console.Error.WriteLine(ex);
        }
    }

    private static void menu()
    {
        Console.Out.WriteLine(
            "usage:\n" +
            "i: send immediate greeting\n" +
            "d: send delayed greeting\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
