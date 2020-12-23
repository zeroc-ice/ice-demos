// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using System.Diagnostics;
using ZeroC.Ice;
using ZeroC.IceGrid;

await using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);
await communicator.ActivateAsync();

// The communicator initialization removes all Ice-related arguments from args.
if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

var hello = IHelloPrx.Parse("hello", communicator);
try
{
    await hello.IcePingAsync();
}
catch (NoEndpointException)
{
    IQueryPrx query = IQueryPrx.Parse("DemoIceGrid/Query", communicator)!;
    hello = query.FindObjectByType("::Demo::Hello")?.Clone(IHelloPrx.Factory) ??
        throw new InvalidOperationException("couldn't find a `::Demo::Hello' object");
}

Menu();

string? line = null;
do
{
    try
    {
        Console.Write("==> ");
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
        Console.WriteLine(ex);
    }
}
while (line != "x");

static void Menu()
{
    Console.WriteLine(
@"usage:
    t: send greeting
    s: shutdown server
    x: exit
    ?: help
");
}
