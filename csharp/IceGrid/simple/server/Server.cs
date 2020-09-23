// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

// using statement - communicator is automatically destroyed at the end of this statement.
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) => communicator.DisposeAsync();

if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

ObjectAdapter adapter = communicator.CreateObjectAdapter("Hello");
string identity = communicator.GetProperty("Identity") ??
    throw new ArgumentException("Identity property not set");
string programName = communicator.GetProperty("Ice.ProgramName") ??
    throw new InvalidOperationException("Ice.ProgramName property not set");
adapter.Add(identity, new Hello(programName));
adapter.Activate();
communicator.WaitForShutdown();
