// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using System.Threading;
using ZeroC.Ice;

await using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);
await communicator.ActivateAsync();

IHelloPrx twoway = communicator.GetPropertyAsProxy("Hello.Proxy", IHelloPrx.Factory) ??
    throw new ArgumentException("invalid proxy");

Console.Write("Say Hello: ");
string? greeting = Console.ReadLine();
Console.Out.WriteLine(twoway.SayHello(greeting));
