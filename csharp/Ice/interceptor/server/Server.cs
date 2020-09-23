// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Collections.Generic;
using System.Configuration;
using ZeroC.Ice;

try
{
    // using statement - communicator is automatically destroyed at the end of this statement
    using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

    // Destroy the communicator on Ctrl+C or Ctrl+Break
    Console.CancelKeyPress += (sender, eventArgs) => communicator.DisposeAsync();

    if (args.Length > 0)
    {
        Console.Error.WriteLine("too many arguments");
        return 1;
    }

    // We use separate object adapters for the authenticator and the thermostat in
    // this demo, so each object has its own endpoints. This way accessibility can
    // be fined tuned on each object separately, through firewall rules for example.
    // It's implemented this way to illustrate best practices, even if the demo
    // doesn't make use of it for simplicity's sake.

    // Create an object adapter for the authenticator.
    ObjectAdapter authenticatorAdapter = communicator.CreateObjectAdapter("Authenticator");
    var authenticator = new Authenticator();
    authenticatorAdapter.Add("authenticator", authenticator);
    authenticatorAdapter.Activate();

    // Set of all the operations to require authorization for.
    var securedOperations = new HashSet<string>(new string[] { "setTemp", "shutdown" });

    // Create an object adapter for the thermostat.
    ObjectAdapter thermostatAdapter = communicator.CreateObjectAdapter("Thermostat");
    thermostatAdapter.Add("thermostat", new Thermostat());
    thermostatAdapter.Activate(
        (request, current, next) =>
        {
            // Check if the operation requires authorization to invoke.
            if (securedOperations.Contains(current.Operation))
            {
                // Validate the client's access token before dispatching to the servant. 'validateToken' throws an
                // exception if the token is invalid or expired.
                if (current.Context.TryGetValue("accessToken", out string? tokenValue))
                {
                    authenticator.ValidateToken(tokenValue);
                }
                else
                {
                    // If the client didn't include an accessToken, throw an exception.
                    throw new AuthorizationException();
                }
            }
            return next(request, current);
        });

    communicator.WaitForShutdown();
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}

return 0;
