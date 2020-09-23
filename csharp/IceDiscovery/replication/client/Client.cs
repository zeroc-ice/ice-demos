// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

// The new communicator is automatically destroyed (disposed) at the end of the using statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// The communicator initialization removes all Ice-related arguments from args
if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

// Get the hello proxy. We configure the proxy to not cache the server connection with the proxy and to disable the
// locator cache. With this configuration, the IceGrid locator will be queried for each invocation on the proxy and
// the invocation will be sent over the server connection matching the returned endpoints.
var hello = IHelloPrx.Parse("ice:hello", communicator);
hello = hello.Clone(cacheConnection: false, locatorCacheTimeout: TimeSpan.Zero);

while (true)
{
    Console.Out.Write("enter the number of iterations: ");
    Console.Out.Flush();
    string? line = Console.In.ReadLine();
    if (line == null || line == "x")
    {
        break;
    }
    int count = int.Parse(line);

    Console.Out.Write("enter the delay between each greetings (in ms): ");
    Console.Out.Flush();
    line = Console.In.ReadLine();
    if (line == null || line == "x")
    {
        break;
    }
    int delay = Convert.ToInt32(line);

    if (delay < 0)
    {
        delay = 500; // 500 milli-seconds
    }

    for (int i = 0; i < count; i++)
    {
        Console.Out.WriteLine(hello.GetGreeting());
        System.Threading.Thread.Sleep(delay);
    }
}
