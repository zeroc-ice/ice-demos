//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;

class Client
{
    static int Main(String[] args)
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

    static int run(Ice.Communicator communicator)
    {
        var context = communicator.getImplicitContext();
        var thermostat = ThermostatPrxHelper.checkedCast(communicator.propertyToProxy("Thermostat.Proxy"));
        if(thermostat == null)
        {
            Console.Error.WriteLine("invalid thermostat proxy");
            return 1;
        }

        var authenticator = AuthenticatorPrxHelper.checkedCast(communicator.propertyToProxy("Authenticator.Proxy"));
        if(authenticator == null)
        {
            Console.Error.WriteLine("invalid authenticator proxy");
            return 1;
        }

        menu();

        string line = null;
        do
        {
            try
            {
                Console.Out.Write("\n==>");
                Console.Out.Flush();
                line = Console.ReadLine();

                switch(line)
                {
                    case "get-temp":
                    {
                        Console.Out.WriteLine("Current temperature is " + thermostat.getTemp());
                        break;
                    }
                    case "set-temp":
                    {
                        try
                        {
                            Console.Out.WriteLine("Enter desired temperature: ");
                            thermostat.setTemp(float.Parse(Console.ReadLine()));
                            Console.Out.WriteLine("New temperature is " + thermostat.getTemp());
                        }
                        catch(FormatException)
                        {
                            Console.Error.WriteLine("Provided temperature is not a parsable float.");
                        }
                        catch(AuthorizationException)
                        {
                            Console.Error.WriteLine("Failed to set temperature. Access denied!");
                        }
                        break;
                    }
                    case "get-token":
                    {
                        Console.Out.WriteLine("Username:");
                        string username = Console.ReadLine();
                        Console.Out.WriteLine("Password:");
                        string password = Console.ReadLine();
                        //
                        // Request an access token from the server's authentication object.
                        //
                        Token token = authenticator.getToken(username, password);
                        Console.Out.WriteLine("Successfully retrieved access token: \"" + token.value + "\"\n");
                        //
                        // Add the access token to the communicator's context, so it will be
                        // sent along with every request made through it.
                        //
                        context.put("accessToken", token.value);
                        break;
                    }
                    case "release-token":
                    {
                        if(context.containsKey("accessToken"))
                        {
                            context.remove("accessToken");
                        }
                        else
                        {
                            Console.Out.WriteLine("There is no access token to release.");
                        }
                        break;
                    }
                    case "shutdown":
                    {
                        thermostat.shutdown();
                        break;
                    }
                    case "x":
                    {
                        // Nothing to do
                        break;
                    }
                    case "?":
                    {
                        menu();
                        break;
                    }
                    default:
                    {
                        Console.Out.WriteLine("Unknown command `" + line + "'");
                        menu();
                        break;
                    }
                }
            }
            catch(System.IO.IOException ex)
            {
                Console.Error.WriteLine(ex);
            }
            catch(Ice.LocalException ex)
            {
                Console.Error.WriteLine(ex);
            }
        }
        while(!line.Equals("x"));

        return 0;
    }

    private static void menu()
    {
        Console.Out.Write(
            "usage:\n" +
            "get-temp: gets the current temperature\n" +
            "set-temp: sets the temperature\n" +
            "get-token: gets a new token from the server\n" +
            "release-token: releases the currently held token\n" +
            "shutdown: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
