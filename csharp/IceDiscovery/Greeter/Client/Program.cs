// Copyright (c) ZeroC, Inc.

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// TBD
var initData = new Ice.InitializationData
{
    properties = new Ice.Properties(ref args)
};
initData.properties.setProperty("Ice.Plugin.Discovery", "IceDiscovery:IceDiscovery.PluginFactory");

// Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
using Ice.Communicator communicator = Ice.Util.initialize(initData);

// Create proxy to the Greeter object hosted by the server. "greeter" is a stringified proxy with no addressing
// information, also known as a well-known proxy. It's resolved by the IceDiscovery plugin.
GreeterPrx greeter = GreeterPrxHelper.createProxy(communicator, "greeter");

// Send a request to the remote object and get the response.
string greeting = await greeter.GreetAsync(Environment.UserName);

Console.WriteLine(greeting);
