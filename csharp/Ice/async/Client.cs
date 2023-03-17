//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;
using System.Threading.Tasks;

public class Client
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            using (Ice.Communicator communicator = Ice.Util.initialize(ref args, "config.client"))
            {
                if (args.Length > 0)
                {
                    Console.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    status = Run(communicator);
                }
            }
        }
        catch (Exception exception)
        {
            Console.WriteLine(exception);
            status = 1;
        }

        return status;
    }

    private static int Run(Ice.Communicator communicator)
    {
        HelloPrx hello = HelloPrxHelper.checkedCast(communicator.propertyToProxy("Hello.Proxy"));
        if (hello == null)
        {
            Console.WriteLine("invalid proxy");
            return 1;
        }

        Menu();

        string line = null;
        do
        {
            try
            {
                Console.Write("==> ");
                Console.Out.Flush();
                line = Console.In.ReadLine();
                switch (line)
                {
                    case null:
                        break;

                    case "i":
                        hello.sayHello(0);
                        break;

                    case "d":
                        _ = HelloAsync(hello);
                        break;

                    case "s":
                        hello.shutdown();
                        break;

                    case "x":
                        break;

                    case "?":
                        Menu();
                        break;

                    default:
                        Console.WriteLine("unknown command `" + line + "'");
                        Menu();
                        break;
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
        }
        while (!line.Equals("x"));

        return 0;
    }

    private static async Task HelloAsync(HelloPrx hello)
    {
        try
        {
            await hello.sayHelloAsync(5000);
        }
        catch (RequestCanceledException)
        {
            Console.WriteLine("RequestCanceledException");
        }
        catch (Exception exception)
        {
            Console.WriteLine("sayHello AMI call failed:");
            Console.WriteLine(exception);
        }
    }

    private static void Menu()
    {
        Console.WriteLine(
            "usage:\n" +
            "i: send immediate greeting\n" +
            "d: send delayed greeting\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
