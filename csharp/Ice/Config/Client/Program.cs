// Copyright (c) ZeroC, Inc.

using System.Diagnostics;

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// Create an Ice communicator to initialize the Ice runtime. The communicator gets its configuration properties from
// file config.client in the client's current working directory. The communicator initialization also parses the
// command-line options to find and set additional properties.
using Ice.Communicator communicator = Ice.Util.initialize(ref args, "config.client");

// We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
// It's null if the property is not set.
GreeterPrx? greeter = GreeterPrxHelper.uncheckedCast(communicator.propertyToProxy("Greeter.Proxy"));
Debug.Assert(greeter is not null);

// Send a request to the remote object and get the response.
string greeting = await greeter.GreetAsync(Environment.UserName);

Console.WriteLine(greeting);
