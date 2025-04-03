// Copyright (c) ZeroC, Inc.

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// Set the Ice.ImplicitContext property to "Shared" before calling Ice.Util.initialize.
// This is only necessary for the implicit context API (see below).
var initData = new Ice.InitializationData
{
    properties = new Ice.Properties(ref args)
};
initData.properties.setProperty("Ice.ImplicitContext", "Shared");

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using Ice.Communicator communicator = Ice.Util.initialize(initData);

// Create a Greeter proxy. If you run the server on a different computer, replace localhost in the string below with
// the server's hostname or IP address.
GreeterPrx greeter = GreeterPrxHelper.createProxy(communicator, "greeter:tcp -h localhost -p 4061");

// Send a request to the remote object and get the response. We request a French greeting by setting the context
// parameter.
string greeting = await greeter.GreetAsync(
    Environment.UserName,
    context: new Dictionary<string, string> { ["language"] = "fr" });

Console.WriteLine(greeting);

// Do it again, this time by setting the context on the proxy.
var greeterEs = GreeterPrxHelper.uncheckedCast(
    greeter.ice_context(new Dictionary<string, string> { ["language"] = "es" }));

greeting = await greeterEs.GreetAsync("alice");
Console.WriteLine(greeting);

// One more time, this time with an implicit context set on the communicator.
communicator.getImplicitContext().put("language", "de");
greeting = await greeter.GreetAsync("bob");
Console.WriteLine(greeting);
