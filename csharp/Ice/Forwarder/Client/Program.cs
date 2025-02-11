// Copyright (c) ZeroC, Inc.

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// We create a Greeter proxy for a Greeter object in the Forwarder server (port 10000).
GreeterPrx greeter = GreeterPrxHelper.createProxy(communicator, "greeter:tcp -h localhost -p 10000");

// Send a request to the remote object and get the response.
string greeting = await greeter.GreetAsync(Environment.UserName);

Console.WriteLine(greeting);
