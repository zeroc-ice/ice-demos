// Copyright (c) ZeroC, Inc.

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Set the default locator of the new communicator. It's the address of the Locator hosted by our IceGrid registry.
// You can also set this proxy with the Ice.Default.Locator property.
communicator.setDefaultLocator(
    Ice.LocatorPrxHelper.createProxy(communicator, "IceGrid/Locator:tcp -h localhost -p 4061"));

// Create a proxy to the Query object hosted by the IceGrid registry. "IceGrid/Query" a well-known proxy, without
// addressing information.
IceGrid.QueryPrx query = IceGrid.QueryPrxHelper.createProxy(communicator, "IceGrid/Query");

// Look up an object with type ::VisitorCenter::Greeter.
string greeterTypeId = GreeterPrxHelper.ice_staticId(); // ::VisitorCenter::Greeter
Ice.ObjectPrx? proxy = await query.findObjectByTypeAsync(greeterTypeId);

if (proxy is null)
{
    Console.WriteLine($"The IceGrid registry doesn't know any object with type '{greeterTypeId}'.");
}
else
{
    // Cast the object proxy to a Greeter proxy.
    GreeterPrx greeter = GreeterPrxHelper.uncheckedCast(proxy);

    // Send a request to the remote object and get the response.
    string greeting = await greeter.GreetAsync(Environment.UserName);
    Console.WriteLine(greeting);

    // Send another request to the remote object. With the default configuration we use for this client, this request
    // reuses the connection and reaches the same server.
    greeting = await greeter.GreetAsync("alice");
    Console.WriteLine(greeting);
}
