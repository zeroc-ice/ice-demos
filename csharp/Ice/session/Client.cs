// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Collections.Generic;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceSessionClient")]
[assembly: AssemblyDescription("Ice session demo client")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class Client
{
    public class App : Ice.Application
    {
        public override int run(string[] args)
        {
            if(args.Length > 0)
            {
                Console.Error.WriteLine(appName() + ": too many arguments");
                return 1;
            }

            string name;
            do
            {
                Console.Out.Write("Please enter your name ==> ");
                Console.Out.Flush();

                name = Console.In.ReadLine();
                if(name == null)
                {
                    return 1;
                }
                name = name.Trim();
            }
            while(name.Length == 0);

            var basePrx = communicator().propertyToProxy("SessionFactory.Proxy");
            var factory = SessionFactoryPrxHelper.checkedCast(basePrx);
            if(factory == null)
            {
                Console.Error.WriteLine("invalid proxy");
                return 1;
            }

            var session = factory.create(name);

            var hellos = new List<HelloPrx>();

            menu();

            bool destroy = true;
            bool shutdown = false;
            while(true)
            {
                Console.Out.Write("==> ");
                Console.Out.Flush();
                string line = Console.In.ReadLine();
                if(line == null)
                {
                    break;
                }
                if(line.Length > 0 && Char.IsDigit(line[0]))
                {
                    int index = Int32.Parse(line);
                    if(index < hellos.Count)
                    {
                        var hello = hellos[index];
                        hello.sayHello();
                    }
                    else
                    {
                        Console.Out.WriteLine("Index is too high. " + hellos.Count +
                                              " hello objects exist so far.\n" +
                                              "Use `c' to create a new hello object.");
                    }
                }
                else if(line.Equals("c"))
                {
                    hellos.Add(session.createHello());
                    Console.Out.WriteLine("Created hello object " + (hellos.Count - 1));
                }
                else if(line.Equals("s"))
                {
                    destroy = false;
                    shutdown = true;
                    break;
                }
                else if(line.Equals("x"))
                {
                    break;
                }
                else if(line.Equals("t"))
                {
                    destroy = false;
                    break;
                }
                else if(line.Equals("?"))
                {
                    menu();
                }
                else
                {
                    Console.Out.WriteLine("Unknown command `" + line + "'.");
                    menu();
                }
            }

            if(destroy)
            {
                session.destroy();
            }
            if(shutdown)
            {
                factory.shutdown();
            }
            return 0;
        }

        private static void menu()
        {
            Console.Out.WriteLine(
                "usage:\n" +
                "c:     create a new per-client hello object\n" +
                "0-9:   send a greeting to a hello object\n" +
                "s:     shutdown the server and exit\n" +
                "x:     exit\n" +
                "t:     exit without destroying the session\n" +
                "?:     help\n");
        }
    }

    public static int Main(string[] args)
    {
        var app = new App();
        return app.main(args, "config.client");
    }
}
