// Copyright (c) ZeroC, Inc.

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Set the default locator of the new communicator. It's the address of the Locator hosted by our IceGrid registry.
// You can also set this proxy with the Ice.Default.Locator property.
communicator.setDefaultLocator(
    Ice.LocatorPrxHelper.createProxy(communicator, "IceGrid/Locator:tcp -h localhost -p 4061"));

// Create proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no addressing
// information, also known as a well-known proxy. The IceGrid registry resolves this well-known proxy and returns the
// actual address (endpoint) of the server to this client.
GreeterPrx greeter = GreeterPrxHelper.createProxy(communicator, "greeter");

// Send a request to the remote object and get the response.
string greeting = await greeter.GreetAsync(Environment.UserName);
Console.WriteLine(greeting);

// Send another request to the remote object. With the default configuration we use for this client, this request
// reuses the connection and reaches the same server, even when we have multiple replicated servers.
greeting = await greeter.GreetAsync("alice");
Console.WriteLine(greeting);
