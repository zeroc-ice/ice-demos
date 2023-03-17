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
                    case "x":
                        break;

                    case "i":
                    case "d":
                        Task _ = HelloAsync(hello, delay: line == "i" ? 0 : 5000);
                        break;

                    case "s":
                        hello.shutdown();
                        break;

                    case "?":
                        Menu();
                        break;

                    default:
                        Console.WriteLine($"unknown command `{line}'");
                        Menu();
                        break;
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
        }
        while (line != "x");

        return 0;
    }

    private static async Task HelloAsync(HelloPrx hello, int delay)
    {
        try
        {
            await hello.sayHelloAsync(delay);
        }
        catch (RequestCanceledException)
        {
            Console.WriteLine("RequestCanceledException");
        }
        catch (Exception exception)
        {
            Console.WriteLine($"sayHello AMI call failed: {exception}");
        }
    }

    private static void Menu()
    {
        Console.WriteLine(
            @"usage:
i: send immediate greeting
d: send delayed greeting
s: shutdown server
x: exit
?: help
");
    }
}
