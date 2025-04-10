// Copyright (c) ZeroC, Inc.

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// Register the IceDiscovery plugin. The plugin will be loaded during communicator initialization.
IceDiscovery.Util.registerIceDiscovery(loadOnInitialize: true);

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// The IceDiscovery plugin is created and initialized by initialize, and sets the default locator on the new
// communicator.
await using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create a proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no addressing
// information, also known as a well-known proxy. It's resolved by the default locator installed by the IceDiscovery
// plugin.
GreeterPrx greeter = GreeterPrxHelper.createProxy(communicator, "greeter");

// Send a request to the remote object and get the response.
string greeting = await greeter.GreetAsync(Environment.UserName);

Console.WriteLine(greeting);
