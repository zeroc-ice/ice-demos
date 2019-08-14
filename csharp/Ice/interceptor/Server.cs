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
                    //
                    var authenticatorAdapter = communicator.createObjectAdapter("Authenticator");
                    var authenticator = new AuthenticatorI();
                    authenticatorAdapter.add(authenticator, Ice.Util.stringToIdentity("authenticator"));
                    authenticatorAdapter.activate();

                    //
                    // List of all the operations to require authorization for.
                    //
                    HashSet<String> securedOperations = new HashSet<String>(new String[] {"setTemp", "shutdown"});
                    //
                    // Create an object adapter for the thermostat.
                    //
                    var thermostatAdapter = communicator.createObjectAdapter("Thermostat");
                    var thermostat = new ThermostatI();
                    thermostatAdapter.add(new InterceptorI(thermostat, authenticator, securedOperations), Ice.Util.stringToIdentity("thermostat"));
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
