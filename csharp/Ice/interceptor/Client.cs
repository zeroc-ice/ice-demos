//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;

public class Client
{
    public static int Main(String[] args)
    {
        int status = 0;

        try
        {
            //
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.client"))
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
                    status = run(communicator);
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

    private static int run(Ice.Communicator communicator)
    {
        var thermostat = ThermostatPrxHelper.checkedCast(communicator.propertyToProxy("Thermostat.Proxy"));
        if(thermostat == null)
        {
            Console.Error.WriteLine("invalid thermostat proxy");
            return 1;
        }

        //
        // Read in values from the command line for the username, password, and
        // the temperature to try and set the thermostat to.
        //
        string username;
        string password;
        float desiredTemp;
        try
        {
            Console.Out.WriteLine("Username:");
            username = Console.ReadLine();
            Console.Out.WriteLine("Password:");
            password = Console.ReadLine();
            Console.Out.WriteLine("Enter desired temperature:");
            desiredTemp = float.Parse(Console.ReadLine());
            Console.Out.WriteLine();
        }
        catch(FormatException)
        {
            Console.Error.WriteLine("Specified temperature is not a float value.");
            return 1;
        }

        //
        // Tries calling 'setTemp' without authorization and receives an exception.
        //
        Console.Out.WriteLine("Attempting to set temperature without access token...");
        try
        {
            Console.Out.WriteLine("\tCurrent temperature is " + thermostat.getTemp());
            thermostat.setTemp(desiredTemp);
            Console.Out.WriteLine("\tNew temperature is " + thermostat.getTemp());
        }
        catch(AuthorizationException)
        {
            Console.Error.WriteLine("\tFailed to set temperature. Access denied!");
        }
        Console.Out.WriteLine();

        //
        // Request an access token from the server's authentication object.
        //
        var authenticator = AuthenticatorPrxHelper.checkedCast(communicator.propertyToProxy("Authenticator.Proxy"));
        if(authenticator == null)
        {
            Console.Error.WriteLine("invalid authenticator proxy");
            return 1;
        }
        Token token = authenticator.getToken(username, password);
        Console.Out.WriteLine("Successfully retrieved access token: \"" + token.value + "\"\n");

        //
        // Add the access token to the communicator's context, so it will be
        // sent along with every request made through it.
        //
        var context = communicator.getImplicitContext();
        context.put("accessToken", token.value);

        //
        // Tries calling 'setTemp' again, this time with the access token.
        //
        Console.Out.WriteLine("Attempting to set temperature with access token...");
        try
        {
            Console.Out.WriteLine("\tCurrent temperature is " + thermostat.getTemp());
            thermostat.setTemp(desiredTemp);
            Console.Out.WriteLine("\tNew temperature is " + thermostat.getTemp());
        }
        catch(AuthorizationException)
        {
            //
            // No longer encountered since the client has authorization to use 'setTemp' now.
            //
            Console.Error.WriteLine("\tFailed to set temperature. Access denied!");
        }
        Console.Out.WriteLine();

        return 0;
    }
}
