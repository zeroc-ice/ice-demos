// Copyright (c) ZeroC, Inc.

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// Configure the communicator to load the IceDiscovery plugin during initialization. This plugin installs a default
// locator on the communicator.
var initData = new Ice.InitializationData
{
    properties = new Ice.Properties(ref args)
};
initData.properties.setProperty("Ice.Plugin.Discovery", "IceDiscovery:IceDiscovery.PluginFactory");

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
using Ice.Communicator communicator = Ice.Util.initialize(initData);

// Create proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no addressing
// information, also known as a well-known proxy. It's resolved by the default locator installed by the IceDiscovery
// plugin.
GreeterPrx greeter = GreeterPrxHelper.createProxy(communicator, "greeter");

// Send a request to the remote object and get the response.
string greeting = await greeter.GreetAsync(Environment.UserName);

Console.WriteLine(greeting);
