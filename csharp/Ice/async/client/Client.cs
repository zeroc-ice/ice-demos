// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

try
{
    // using statement - communicator is automatically destroyed at the end of this statement
    using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

    // Destroy the communicator on Ctrl+C or Ctrl+Break
    Console.CancelKeyPress += (sender, eventArgs) => communicator.Dispose();

    if (args.Length > 0)
    {
        Console.Error.WriteLine("too many arguments");
        return 1;
    }

    IHelloPrx hello = communicator.GetPropertyAsProxy("Hello.Proxy", IHelloPrx.Factory) ??
        throw new ArgumentException("invalid proxy");

    Menu();

    string? line = null;
    do
    {
        try
        {
            Console.Out.Write("==> ");
            Console.Out.Flush();
            line = Console.In.ReadLine();
            if (line == null)
            {
                break;
            }
            if (line == "i")
            {
                hello.SayHello(0);
            }
            else if (line == "d")
            {
                await hello.SayHelloAsync(5000);
            }
            else if (line == "s")
            {
                hello.Shutdown();
            }
            else if (line == "x")
            {
                // Nothing to do
            }
            else if (line == "?")
            {
                Menu();
            }
            else
            {
                Console.Out.WriteLine($"unknown command `{line}'");
                Menu();
            }
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
        }
    }
    while (line != "x");
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}
return 0;

static void Menu()
{
    Console.Out.WriteLine("usage:\n" +
                          "i: send immediate greeting\n" +
                          "d: send delayed greeting\n" +
                          "s: shutdown server\n" +
                          "x: exit\n" +
                          "?: help\n");
}
