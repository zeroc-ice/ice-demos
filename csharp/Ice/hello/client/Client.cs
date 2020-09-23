// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using System.Threading;
using ZeroC.Ice;

// The new communicator is automatically destroyed (disposed) at the end of the using statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// The communicator initialization removes all Ice-related arguments from args
if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

IHelloPrx twoway = communicator.GetPropertyAsProxy("Hello.Proxy", IHelloPrx.Factory) ??
    throw new ArgumentException("invalid proxy");

bool secure = false;
int timeout = -1;
int delay = 0;

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
            var source = new CancellationTokenSource(TimeSpan.FromMilliseconds(timeout));
            twoway.SayHello(delay, cancel: source.Token);
        }
        else if (line == "T")
        {
            if (timeout == -1)
            {
                timeout = 2000;
                Console.WriteLine("timeout is now set to 2000ms");
            }
            else
            {
                timeout = -1;
                Console.WriteLine("timeout is now switched off");
            }
        }
        else if (line == "P")
        {
            if (delay == 0)
            {
                delay = 2500;
                Console.WriteLine("server delay is now set to 2500ms");
            }
            else
            {
                delay = 0;
                Console.WriteLine("server delay is now deactivated");
            }
        }
        else if (line == "S")
        {
            secure = !secure;

            twoway = twoway.Clone(preferNonSecure: !secure);
            Console.WriteLine(secure ? "secure mode is now on" : "secure mode is now off");
        }
        else if (line == "s")
        {
            twoway.Shutdown();
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
    T: set a timeout
    P: set a server delay
    S: switch secure mode on/off
    s: shutdown server
    x: exit
    ?: help
");
}
