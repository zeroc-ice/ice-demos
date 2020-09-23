// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using System.Diagnostics;
using ZeroC.Ice;

// using statement - communicator is automatically destroyed at the end of this statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) => communicator.DisposeAsync();

if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

IPingPrx ping = communicator.GetPropertyAsProxy("Ping.Proxy", IPingPrx.Factory) ??
    throw new ArgumentException("invalid proxy");

// A method needs to be invoked thousands of times before the JIT compiler will convert it to native code.
// To ensure an accurate latency measurement, we need to "warm up" the JIT compiler.

Console.Error.Write("warming up the JIT compiler...");
Console.Error.Flush();
for (int i = 0; i < 20000; i++)
{
    ping.IcePing();
}
Console.Error.WriteLine("ok");

var watch = new Stopwatch();
watch.Start();
double repetitions = 100000;
Console.Out.WriteLine($"pinging server {repetitions} times (this may take a while)");
for (int i = 0; i < repetitions; i++)
{
    ping.IcePing();
}
watch.Stop();

Console.Out.WriteLine($"time for {repetitions} pings: {watch.ElapsedMilliseconds}ms");
Console.Out.WriteLine($"time per ping: {watch.ElapsedMilliseconds / repetitions}ms");
