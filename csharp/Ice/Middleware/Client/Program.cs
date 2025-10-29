// Copyright (c) ZeroC, Inc.

// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using var communicator = new Ice.Communicator(ref args);

// Create a Greeter proxy. If you run the server on a different computer, replace localhost in the string below with
// the server's hostname or IP address.
GreeterPrx greeter = GreeterPrxHelper.createProxy(communicator, "greeter:tcp -h localhost -p 4061");

// We hardcode the tokens in this demo, for simplicity. A real application would obtain the token from a secure source.
string validToken = "iced tea";

// Send a request to the remote object with an invalid token in the request context.
try
{
    string unexpected = await greeter.GreetAsync(
        Environment.UserName,
        context: new Dictionary<string, string> { ["token"] = "pineapple" });

    Console.WriteLine($"Received unexpected greeting: {unexpected}");
}
catch (Ice.DispatchException dispatchException) when (dispatchException.replyStatus is Ice.ReplyStatus.Unauthorized)
{
    // Expected with an invalid (or missing) token. See AuthorizationMiddleware.
}

// Send a request with the correct token in the request context.
string greeting = await greeter.GreetAsync(
    Environment.UserName,
    context: new Dictionary<string, string> { ["token"] = validToken });

Console.WriteLine(greeting);
