// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

// The new communicator is automatically destroyed (disposed) at the end of the using statement.
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) => communicator.DisposeAsync();

// The communicator initialization removes all Ice-related arguments from args
if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

var hello = IHelloPrx.Parse("hello", communicator);
hello.SayHello();
