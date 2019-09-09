//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.ImplicitContext;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        List<String> extraArgs = new ArrayList<>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.client", extraArgs))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.interceptor");

            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                status = run(communicator);
            }
        }

        System.exit(status);
    }

    private static int run(Communicator communicator)
    {
        ThermostatPrx thermostat =
            ThermostatPrx.checkedCast(communicator.propertyToProxy("Thermostat.Proxy"));
        if(thermostat == null)
        {
            System.err.println("invalid thermostat proxy");
            return 1;
        }
        AuthenticatorPrx authenticator =
            AuthenticatorPrx.checkedCast(communicator.propertyToProxy("Authenticator.Proxy"));
        if(authenticator == null)
        {
            System.err.println("invalid authenticator proxy");
            return 1;
        }

        menu();

        try(BufferedReader in = new BufferedReader(new InputStreamReader(System.in)))
        {
            String line = null;
            do
            {
                System.out.print("\n==> ");
                System.out.flush();
                line = in.readLine();
                if(line == null)
                {
                    break;
                }

                switch(line)
                {
                    case "1":
                    {
                        System.out.println("Current temperature is " + thermostat.getTemp());
                        break;
                    }
                    case "2":
                    {
                        try
                        {
                            System.out.println("Enter desired temperature: ");
                            thermostat.setTemp(Float.parseFloat(in.readLine()));
                            System.out.println("New temperature is " + thermostat.getTemp());
                        }
                        catch(NumberFormatException ex)
                        {
                            System.err.println("Provided temperature is not a parsable float.");
                        }
                        catch(TokenExpiredException ex)
                        {
                            System.err.println("Failed to set temperature. Token expired!");
                        }
                        catch(AuthorizationException ex)
                        {
                            System.err.println("Failed to set temperature. Access denied!");
                        }
                        break;
                    }
                    case "3":
                    {
                        //
                        // Request an access token from the server's authentication object.
                        //
                        String token = authenticator.getToken();
                        System.out.println("Successfully retrieved access token: \"" + token + "\"");
                        //
                        // Add the access token to the communicator's context, so it will be
                        // sent along with every request made through it.
                        //
                        communicator.getImplicitContext().put("accessToken", token);
                        break;
                    }
                    case "4":
                    {
                        ImplicitContext context = communicator.getImplicitContext();
                        if(context.containsKey("accessToken"))
                        {
                            context.remove("accessToken");
                        }
                        else
                        {
                            System.out.println("There is no access token to release.");
                        }
                        break;
                    }
                    case "s":
                    {
                        try
                        {
                            thermostat.shutdown();
                        }
                        catch(TokenExpiredException ex)
                        {
                            System.err.println("Failed to shutdown thermostat. Token expired!");
                        }
                        catch(AuthorizationException ex)
                        {
                            System.err.println("Failed to shutdown thermostat. Access denied!");
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
                        menu();
                        break;
                    }
                    default:
                    {
                        System.out.println("Unknown command `" + line + "'");
                        menu();
                        break;
                    }
                }
            }
            while(!line.equals("x"));
        }
        catch(java.io.IOException|com.zeroc.Ice.LocalException ex)
        {
            ex.printStackTrace();
        }

        return 0;
    }

    private static void menu()
    {
        System.out.println(
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
