// Copyright (c) ZeroC, Inc.

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// Configure the communicator to load the IceLocatorDiscovery plugin during initialization. This plugin will discover
// the locator (IceGrid registry in this demo) to use. As a result, we don't need to configure the default locator on
// this communicator.
var initData = new Ice.InitializationData
{
    properties = new Ice.Properties(ref args)
};
initData.properties.setProperty("Ice.Plugin.LocatorDiscovery", "IceLocatorDiscovery:IceLocatorDiscovery.PluginFactory");

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
using Ice.Communicator communicator = Ice.Util.initialize(initData);

// Create a proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no addressing
// information, also known as a well-known proxy. It's specified by the <object> element in the IceGrid XML file.
// The IceGrid registry resolves this well-known proxy and returns the actual address (endpoint) of the server to this
// client.
GreeterPrx greeter = GreeterPrxHelper.createProxy(communicator, "greeter");

// Send a request to the remote object and get the response.
string greeting = await greeter.GreetAsync(Environment.UserName);
Console.WriteLine(greeting);

// Send another request to the remote object. With the default configuration we use for this client, this request
// reuses the connection and reaches the same server, even when we have multiple replicated servers.
greeting = await greeter.GreetAsync("alice");
Console.WriteLine(greeting);
