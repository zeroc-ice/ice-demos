// Copyright (c) ZeroC, Inc.

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using System.Diagnostics;
using VisitorCenter;

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is derived from
// the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
Glacier2.RouterPrx router =
    Glacier2.RouterPrxHelper.createProxy(communicator, "Glacier2/router:tcp -h localhost -p 4063");

// Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
// username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime of the
// session is the same as the lifetime of the connection.
Glacier2.SessionPrx? session = await router.createSessionAsync(Environment.UserName, "password");

// The proxy returned by createSession is null because we did not configure a SessionManager on the Glacier2 router.
Debug.Assert(session is null);

// Create a proxy to the Greeter object in the server behind the Glacier2 router. Typically, the client cannot connect
// directly to this server because it's on an unreachable network.
GreeterPrx greeter = GreeterPrxHelper.createProxy(communicator, "greeter:tcp -h localhost -p 4061");

// Configure the proxy to route requests using the Glacier2 router.
greeter = GreeterPrxHelper.uncheckedCast(greeter.ice_router(router));

// Send a request to the remote object and get the response.
string greeting = await greeter.GreetAsync(Environment.UserName);
Console.WriteLine(greeting);

// Send a second request to observe the effect in the Glacier2 router log.
greeting = await greeter.GreetAsync("alice");
Console.WriteLine(greeting);
