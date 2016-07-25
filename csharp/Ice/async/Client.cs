// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Reflection;
using System.Threading.Tasks;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceAsyncClient")]
[assembly: AssemblyDescription("Ice async demo client")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class Client
{
    public class App : Ice.Application
    {
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

        private async void helloAsync(HelloPrx hello)
        {
            try
            {
                await hello.sayHelloAsync(5000);
            }
            catch(AggregateException ae)
            {
                if(ae.InnerException is RequestCanceledException)
                {
                    Console.Error.WriteLine("RequestCanceledException");
                }
                else
                {
                    Console.Error.WriteLine("sayHello AMI call failed:");
                    Console.Error.WriteLine(ae.InnerException);
                }
            }
        }

        public override int run(string[] args)
        {
            if(args.Length > 0)
            {
                Console.Error.WriteLine(appName() + ": too many arguments");
                return 1;
            }

            var hello = HelloPrxHelper.checkedCast(communicator().propertyToProxy("Hello.Proxy"));
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
    }

    public static int Main(string[] args)
    {
        var app = new App();
        return app.main(args, "config.client");
    }
}
