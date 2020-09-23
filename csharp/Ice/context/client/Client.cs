// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Collections.Generic;
using System.Configuration;
using ZeroC.Ice;

try
{
    // The new communicator is automatically destroyed (disposed) at the end of the using statement
    using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

    // The communicator initialization removes all Ice-related arguments from args
    if (args.Length > 0)
    {
        Console.Error.WriteLine("too many arguments");
        return 1;
    }
    IContextPrx proxy = communicator.GetPropertyAsProxy("Context.Proxy", IContextPrx.Factory) ??
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
            if (line == "1")
            {
                proxy.Call();
            }
            else if (line == "2")
            {
                var ctx = new Dictionary<string, string>() { { "type", "Explicit" } };
                proxy.Call(ctx);
            }
            else if (line == "3")
            {
                proxy.Clone(context: new Dictionary<string, string>() { { "type", "Per-Proxy" } }).Call();
            }
            else if (line == "4")
            {
                Dictionary<string, string> currentContext = communicator.CurrentContext;
                communicator.CurrentContext = new Dictionary<string, string>() { { "type", "Implicit" } };
                proxy.Call();
                communicator.CurrentContext = currentContext;
            }
            else if (line == "s")
            {
                proxy.Shutdown();
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
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}
return 0;

static void Menu()
{
    Console.Write(
@"usage:
    1: call with no request context
    2: call with explicit request context
    3: call with per-proxy request context
    4: call with implicit request context
    s: shutdown server
    x: exit
    ?: help
");
}
