// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

// using statement - communicator is automatically destroyed at the end of this statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// The communicator intialization removes all Ice-related arguments from args.
if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

IThermostatPrx thermostat = communicator.GetPropertyAsProxy("Thermostat.Proxy", IThermostatPrx.Factory) ??
    throw new ArgumentException("invalid thermostat proxy");

IAuthenticatorPrx authenticator =
    communicator.GetPropertyAsProxy("Authenticator.Proxy", IAuthenticatorPrx.Factory) ??
    throw new ArgumentException("invalid authenticator proxy");

Menu();

string? line = null;
do
{
    Console.Out.Write("\n==> ");
    Console.Out.Flush();
    line = Console.ReadLine();

    switch (line)
    {
        case "1":
        {
            Console.Out.WriteLine($"Current temperature is {thermostat.GetTemp()}");
            break;
        }
        case "2":
        {
            try
            {
                string? value = Console.ReadLine();
                if (value == null)
                {
                    break;
                }
                Console.Out.WriteLine("Enter desired temperature: ");
                thermostat.SetTemp(float.Parse(value));
                Console.Out.WriteLine($"New temperature is {thermostat.GetTemp()}");
            }
            catch (FormatException)
            {
                Console.Error.WriteLine("Provided temperature can not be parsed as a float.");
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
            string token = authenticator.GetToken();
            Console.Out.WriteLine($"Successfully retrieved access token: \"{token}\"");

            // Add the access token to the communicator's context, so it will be
            // sent along with every request made through it.
            communicator.CurrentContext["accessToken"] = token;
            break;
        }
        case "4":
        {
            if (communicator.CurrentContext.ContainsKey("accessToken"))
            {
                communicator.CurrentContext.Remove("accessToken");
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
                thermostat.Shutdown();
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
            Console.Out.WriteLine($"Unknown command `{line}'");
            Menu();
            break;
        }
    }
}
while (line != "x");

static void Menu()
{
    Console.Out.Write(
@"usage:
    1: gets the current temperature
    2: sets the temperature
    3: gets a new token from the server
    4: releases the currently held token
    s: shutdown server
    x: exit
    ?: help\n");
}
