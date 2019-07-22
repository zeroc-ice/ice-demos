//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<String>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator =
            com.zeroc.Ice.Util.initialize(args, "config.client", extraArgs))
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

    private static int run(com.zeroc.Ice.Communicator communicator)
    {
        ThermostatPrx thermostat =
            ThermostatPrx.checkedCast(communicator.propertyToProxy("Thermostat.Proxy"));
        if(thermostat == null)
        {
            System.err.println("invalid thermostat proxy");
            return 1;
        }

        //
        // Read in values from the command line for the username, password, and
        // the temperature to try and set the thermostat to.
        //
        String username;
        String password;
        float desiredTemp;
        try(java.util.Scanner userIn = new java.util.Scanner(System.in))
        {
            System.out.println("Username:");
            username = userIn.nextLine();
            System.out.println("Password:");
            password = userIn.nextLine();
            System.out.println("Enter desired temperature:");
            desiredTemp = Float.parseFloat(userIn.nextLine());
            System.out.println();
        }
        catch(NumberFormatException ex)
        {
            System.err.println("Specified temperature is not a valid float.");
            return 1;
        }

        //
        // Tries calling 'setTemp' without authorization and receives an exception.
        //
        System.out.println("Attempting to set temperature without access token...");
        try
        {
            System.out.println("\tCurrent temperature is " + thermostat.getTemp());
            thermostat.setTemp(desiredTemp);
            System.out.println("\tNew temperature is " + thermostat.getTemp());
        }
        catch(AuthorizationException ex)
        {
            System.err.println("\tFailed to set temperature. Access denied!");
        }
        System.out.println();

        //
        // Request an access token from the server's authentication object.
        //
        AuthenticatorPrx authenticator =
            AuthenticatorPrx.checkedCast(communicator.propertyToProxy("Authenticator.Proxy"));
        if(authenticator == null)
        {
            System.err.println("invalid authenticator proxy");
            return 1;
        }
        Token token = authenticator.getToken(username, password);
        System.out.println("Successfully retrieved access token: \"" + token.value + "\"\n");

        //
        // Add the access token to the communicator's context, so it will be
        // sent along with every request made through it.
        //
        com.zeroc.Ice.ImplicitContext context = communicator.getImplicitContext();
        context.put("accessToken", token.value);

        //
        // Tries calling 'setTemp' again, this time with the access token.
        //
        System.out.println("Attempting to set temperature with access token...");
        try
        {
            System.out.println("\tCurrent temperature is " + thermostat.getTemp());
            thermostat.setTemp(desiredTemp);
            System.out.println("\tNew temperature is " + thermostat.getTemp());
        }
        catch(AuthorizationException ex)
        {
            //
            // No longer encountered since the client has authorization to use 'setTemp' now.
            //
            System.err.println("\tFailed to set temperature. Access denied!");
        }

        return 0;
    }
}
