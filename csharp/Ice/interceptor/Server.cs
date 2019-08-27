//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using System;
using System.Collections.Generic;

class Server
{
    static int Main(string[] args)
    {
        int status = 0;

        try
        {
            //
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.server"))
            {
                //
                // Destroy the communicator on Ctrl+C or Ctrl+Break
                //
                Console.CancelKeyPress += (sender, eventArgs) => communicator.destroy();

                if(args.Length > 0)
                {
                    Console.Error.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    //
                    // Create an object adapter for the authenticator.
                    // We use separate adapters to allow for fine-grained control of the endpoints;
                    // Only secure endpoints for the authenticator, and non-secure endpoints for
                    // the thermostat. This demo doesn't make use of this, but is implemented this
                    // way regardless to illustrate best practices.
                    //
                    var authenticatorAdapter = communicator.createObjectAdapter("Authenticator");
                    var authenticator = new AuthenticatorI();
                    authenticatorAdapter.add(authenticator, Ice.Util.stringToIdentity("authenticator"));
                    authenticatorAdapter.activate();

                    //
                    // List of all the operations to require authorization for.
                    //
                    HashSet<string> securedOperations = new HashSet<string>(new String[] {"setTemp", "shutdown"});
                    //
                    // Create an object adapter for the thermostat.
                    //
                    var thermostatAdapter = communicator.createObjectAdapter("Thermostat");
                    var interceptor = new InterceptorI(new ThermostatI(), authenticator, securedOperations);
                    thermostatAdapter.add(interceptor, Ice.Util.stringToIdentity("thermostat"));
                    thermostatAdapter.activate();

                    communicator.waitForShutdown();
                }
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }
}
