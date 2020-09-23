// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

// The new communicator is automatically destroyed (disposed) at the end of the using statement.
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// The communicator initialization removes all Ice-related arguments from args
if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
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
        if (line == "t")
        {
            hello.SayHello();
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
            Console.WriteLine($"unknown command `{line}'");
            Menu();
        }
    }
    catch (Exception ex)
    {
        Console.Error.WriteLine(ex);
    }
}
while (line != "x");

static void Menu()
{
    Console.Write(
@"usage:
    t: send greeting as twoway
    s: shutdown server
    x: exit
    ?: help
");
}
