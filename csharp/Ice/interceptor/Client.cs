// Copyright (c) ZeroC, Inc.

using Demo;

internal class Client
{
    private static int Main(string[] args)
    {
        int status = 0;

        try
        {
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            using var communicator = Ice.Util.initialize(ref args, "config.client");
            // The communicator initialization removes all Ice-related arguments from args.
            if (args.Length > 0)
            {
                Console.Error.WriteLine("too many arguments");
                status = 1;
            }
            else
            {
                status = Run(communicator);
            }
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int Run(Ice.Communicator communicator)
    {
        var thermostat = ThermostatPrxHelper.checkedCast(communicator.propertyToProxy("Thermostat.Proxy"));
        if (thermostat == null)
        {
            Console.Error.WriteLine("invalid thermostat proxy");
            return 1;
        }
        var authenticator = AuthenticatorPrxHelper.checkedCast(communicator.propertyToProxy("Authenticator.Proxy"));
        if (authenticator == null)
        {
            Console.Error.WriteLine("invalid authenticator proxy");
            return 1;
        }

        Menu();

        try
        {
            string line = null;
            do
            {
                Console.Out.Write("\n==> ");
                Console.Out.Flush();
                line = Console.ReadLine();

                switch (line)
                {
                    case "1":
                    {
                        Console.Out.WriteLine("Current temperature is " + thermostat.getTemp());
                        break;
                    }
                    case "2":
                    {
                        try
                        {
                            Console.Out.WriteLine("Enter desired temperature: ");
                            thermostat.setTemp(float.Parse(Console.ReadLine()));
                            Console.Out.WriteLine("New temperature is " + thermostat.getTemp());
                        }
                        catch (FormatException)
                        {
                            Console.Error.WriteLine("Provided temperature is not a parsable float.");
                        }
                        catch (TokenExpiredException)
                        {
                            Console.Error.WriteLine("Failed to set temperature. Token expired!");
                        }
                        catch (AuthorizationException)
                        {
                            Console.Error.WriteLine("Failed to set temperature. Access denied!");
                        }
                        break;
                    }
                    case "3":
                    {
                        // Request an access token from the server's authentication object.
                        string token = authenticator.getToken();
                        Console.Out.WriteLine("Successfully retrieved access token: \"" + token + "\"");
                        // Add the access token to the communicator's context, so it will be
                        // sent along with every request made through it.
                        communicator.getImplicitContext().put("accessToken", token);
                        break;
                    }
                    case "4":
                    {
                        var context = communicator.getImplicitContext();
                        if (context.containsKey("accessToken"))
                        {
                            context.remove("accessToken");
                        }
                        else
                        {
                            Console.Out.WriteLine("There is no access token to release.");
                        }
                        break;
                    }
                    case "s":
                    {
                        try
                        {
                            thermostat.shutdown();
                        }
                        catch (TokenExpiredException)
                        {
                            Console.Error.WriteLine("Failed to shutdown thermostat. Token expired!");
                        }
                        catch (AuthorizationException)
                        {
                            Console.Error.WriteLine("Failed to shutdown thermostat. Access denied!");
                        }
                        break;
                    }
                    case "x":
                    {
                        // Nothing to do, the loop will exit on its own.
                        break;
                    }
                    case "?":
                    {
                        Menu();
                        break;
                    }
                    default:
                    {
                        Console.Out.WriteLine("Unknown command `" + line + "'");
                        Menu();
                        break;
                    }
                }
            }
            while (!line.Equals("x"));
        }
        catch (System.IO.IOException ex)
        {
            Console.Error.WriteLine(ex);
        }
        catch (Ice.LocalException ex)
        {
            Console.Error.WriteLine(ex);
        }

        return 0;
    }

    private static void Menu()
    {
        Console.Out.Write(
            "usage:\n" +
            "1: gets the current temperature\n" +
            "2: sets the temperature\n" +
            "3: gets a new token from the server\n" +
            "4: releases the currently held token\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
