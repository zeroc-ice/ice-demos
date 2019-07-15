//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.oauth.Demo.*;

public class Client
{
    // Per-application ID for OAuth validation.
    public static final String CLIENT_ID = "a380f08e6cbb2114192ae0972a29e57a";

    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.client", extraArgs))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.oauth");

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
            System.err.println("invalid proxy");
            return 1;
        }

        //
        // Read in values from the command line.
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
            System.err.println("Error: Specified temperature is not a valid float.");
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
        // Request an access token from the OAuth provider.
        //
        String host = communicator.getProperties().getProperty("Ice.Default.Host");
        int port = Integer.parseInt(communicator.getProperties().getProperty("OAuth.Provider.Port"));
        java.util.Map<String, String> params = new java.util.HashMap<String, String>();
        params.put("grant_type", "password");
        params.put("username", username);
        params.put("password", password);
        params.put("client_id", CLIENT_ID);

        java.util.Map<String, String> response = sendPostRequest("http://" + host + ":" + port, params);
        String token = response.get("access_token");
        if(token == null)
        {
            System.err.println("Failed to retrieve access token.");
            return 1;
        }
        System.out.println("Issued access token from provider: \"" + token + "\"\n");

        //
        // Add the access token to the proxy's context, so it will be
        // sent along with every request made through the proxy.
        //
        java.util.Map<String, String> context = thermostat.ice_getContext();
        if(context == null)
        {
            context = new java.util.HashMap<String, String>();
        }
        context.put("accessToken", token);
        thermostat = thermostat.ice_context(context);

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
            // No longer thrown since the client has authorization to use 'setTemp' now.
            //
            System.err.println("\tFailed to set temperature. Access denied!");
        }

        return 0;
    }

    private static java.util.Map<String, String> sendPostRequest(String urlString, java.util.Map<String, String> params)
    {
        java.util.Map<String, String> responseParameters = new java.util.HashMap<String, String>();
        try
        {
            //
            // Create the POST request.
            //
            StringBuilder request = new StringBuilder();
            for(java.util.Map.Entry<String, String> param : params.entrySet())
            {
                if(request.length() != 0)
                {
                    request.append('&');
                }
                request.append(java.net.URLEncoder.encode(param.getKey(), "UTF-8"));
                request.append('=');
                request.append(java.net.URLEncoder.encode(param.getValue(), "UTF-8"));
            }
            byte[] data = request.toString().getBytes("UTF-8");

            //
            // Open a connection to the OAuth server and send the request
            //
            java.net.URL url = new java.net.URL(urlString);
            java.net.HttpURLConnection connection = (java.net.HttpURLConnection)url.openConnection();
            connection.setRequestMethod("POST");
            connection.setRequestProperty("charset", "utf-8");
            connection.setRequestProperty("Content-Length", Integer.toString(data.length));
            connection.setUseCaches(false);
            connection.setDoOutput(true);
            connection.getOutputStream().write(data);

            //
            // Read the response.
            //
            int responseCode = connection.getResponseCode();
            byte[] responseData = new byte[connection.getContentLength()];
            if(responseCode == 200)
            {
                connection.getInputStream().read(responseData);
            }
            else
            {
                connection.getErrorStream().read(responseData);
                System.err.println("Server returned error code: " + responseCode);
                String response = new String(responseData, "UTF-8");
                System.err.println("Response:\n" + response);
                return responseParameters;
            }
            String response = new String(responseData, "UTF-8");

            //
            // Parse the response into parameters.
            //
            for(String line : response.split("\r\n"))
            {
                line = line.trim();
                int delimIndex = line.indexOf(':');
                if(delimIndex > 0)
                {
                    // Remove any commas at the end of the line, or leading/trailing whitespace.
                    if(line.charAt(line.length() - 1) == ',')
                    {
                        line = line.substring(0, (line.length() - 1));
                    }

                    // Remove any wrapping quotes.
                    String parameter = line.substring(0, delimIndex).replaceAll("^\"|\"$", "");
                    String value = line.substring(delimIndex + 1).replaceAll("^\"|\"$", "");
                    responseParameters.put(parameter, value);
                }
            }
        }
        catch(java.net.MalformedURLException ex)
        {
            System.err.println("Invalid provider url.");
        }
        catch(java.io.IOException ex)
        {
            System.err.println("Error occured while requesting access token.");
            ex.printStackTrace();
        }
        return responseParameters;
    }
}
