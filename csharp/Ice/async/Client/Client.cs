//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;
using System.Collections.Generic;
using ZeroC.Ice;

try
{
    var properties = new Dictionary<string, string>();
    properties.LoadIceConfigFile("config.client");
    properties.ParseIceArgs(ref args);

    // using statement - communicator is automatically destroyed at the end of this statement
    using var communicator = new Communicator(properties);

    // Destroy the communicator on Ctrl+C or Ctrl+Break
    Console.CancelKeyPress += (sender, eventArgs) => communicator.Dispose();

    if(args.Length > 0)
    {
        Console.Error.WriteLine("too many arguments");
        Environment.Exit(1);
    }

    IHelloPrx hello = communicator.GetPropertyAsProxy("Hello.Proxy", IHelloPrx.Factory) ??
        throw new ArgumentException("invalid proxy");

    Menu();

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
                hello.SayHello(0);
            }
            else if(line.Equals("d"))
            {
                await hello.SayHelloAsync(5000);
            }
            else if(line.Equals("s"))
            {
                hello.Shutdown();
            }
            else if(line.Equals("x"))
            {
                // Nothing to do
            }
            else if(line.Equals("?"))
            {
                Menu();
            }
            else
            {
                Console.Out.WriteLine($"unknown command `{line}'");
                Menu();
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
        }
    }
    while(!line.Equals("x"));
}
catch(Exception ex)
{
    Console.Error.WriteLine(ex);
    Environment.Exit(1);
}

static void Menu()
{
    Console.Out.WriteLine("usage:\n" +
                          "i: send immediate greeting\n" +
                          "d: send delayed greeting\n" +
                          "s: shutdown server\n" +
                          "x: exit\n" +
                          "?: help\n");
}