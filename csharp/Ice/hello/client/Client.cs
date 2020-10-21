// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using System.Threading;
using ZeroC.Ice;

// The new communicator is automatically destroyed (disposed) at the end of the using statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

IHelloPrx twoway = communicator.GetPropertyAsProxy("Hello.Proxy", IHelloPrx.Factory) ??
    throw new ArgumentException("invalid proxy");

Console.Out.WriteLine(twoway.SayHello(greeting));
