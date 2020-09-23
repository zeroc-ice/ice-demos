// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Configuration;
using System.IO;
using ZeroC.Ice;

Console.WriteLine($"The current directory is {Directory.GetCurrentDirectory()}");

// using statement - communicator is automatically destroyed at the end of this statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) => communicator.DisposeAsync();

if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

communicator.WaitForShutdown();
