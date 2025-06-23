// Copyright (c) ZeroC, Inc.

using System.Diagnostics;

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// Create Ice properties from the contents of the config.client file in the current working directory.
var properties = new Ice.Properties();
properties.load("config.client");

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// The communicator gets its properties from initData.properties; Ice.* properties and other reserved properties set in
// args override these properties.
await using Ice.Communicator communicator = Ice.Util.initialize(
    ref args,
    new Ice.InitializationData { properties = properties });

// We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
// It's null if the property is not set.
GreeterPrx? greeter = GreeterPrxHelper.uncheckedCast(communicator.propertyToProxy("Greeter.Proxy"));
Debug.Assert(greeter is not null);

// Send a request to the remote object and get the response.
string greeting = await greeter.GreetAsync(Environment.UserName);

Console.WriteLine(greeting);
