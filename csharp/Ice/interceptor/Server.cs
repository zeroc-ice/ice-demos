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
                    // We use separate object adapters for the authenticator and the thermostat in
                    // this demo, so each object has its own endpoints. This way accessibility can
                    // be fined tuned on each object separately, through firewall rules for example.
                    // It's implemented this way to illustrate best practices, even if the demo
                    // doesn't make use of it for simplicity's sake.

                    //
                    // Create an object adapter for the authenticator.
                    //
                    var authenticatorAdapter = communicator.createObjectAdapter("Authenticator");
                    var authenticator = new AuthenticatorI();
                    authenticatorAdapter.add(authenticator, Ice.Util.stringToIdentity("authenticator"));
                    authenticatorAdapter.activate();

                    //
                    // Set of all the operations to require authorization for.
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
