//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.oauth.Demo.*;

public class Client
{
    // Per-application ID for OAuth validation.
    private static final String CLIENT_ID = "a380f08e6cbb2114192ae0972a29e57a";

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
        // Tries calling 'setTemp' without authorization and receives an exception.
        //
        try
        {
            System.out.println("Current temperature is " + thermostat.getTemp());
            thermostat.setTemp(20.2f);
            System.out.println("New temperature is " + thermostat.getTemp());
        }
        catch(AuthorizationException ex)
        {
            System.err.println("Failed to set temperature. Access denied!");
        }

        //
        // Request an access token from the OAuth provider.
        //
        String host = communicator.getProperties().getProperty("Ice.Default.Host");
        int port = Integer.parseInt(communicator.getProperties().getProperty("OAuth.Provider.Port"));
        java.util.Map<String, String> params = new java.util.HashMap<String, String>();
        params.put("grant_type", "password");
        params.put("username", "admin");
        params.put("password", "1234good");
        params.put("client_id", CLIENT_ID);
        String response = sendPostRequest("http://" + host + ":" + port, params);
        if(response == null)
        {
            System.err.println("Failed to retreive token from provider server.");
            return 1;
        }
        System.out.println(response);//TODO THIS TOO!
        String token = "token?";// TODO REQUEST THE ACCESS TOKEN HERE!

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
        try
        {
            System.out.println("Current temperature is " + thermostat.getTemp());
            thermostat.setTemp(19.8f);
            System.out.println("New temperature is " + thermostat.getTemp());
        }
        catch(AuthorizationException ex)
        {
            System.err.println("Failed to set temperature. Access denied!");
        }

        return 0;
    }

    private static String sendPostRequest(String urlString, java.util.Map<String, String> params)
    {
        try
        {
            //
            // Create the POST payload data.
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
            byte[] data = request.append('\n').toString().getBytes("UTF-8");//TODO REMOVE THE NEED FOR THE LAST NEWLINE?

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
            try(java.io.DataOutputStream out = new java.io.DataOutputStream(connection.getOutputStream()))
            {
                out.write(data);
            }

            //
            // Read the response and return it as a string.
            //
            int responseCode = connection.getResponseCode();
            StringBuilder response = new StringBuilder();
            try(java.io.BufferedReader in =
                new java.io.BufferedReader(new java.io.InputStreamReader(connection.getInputStream(), "UTF-8")))
            {
                int c;
                do
                {
                    c = in.read();
                    response.append((char)c);
                } while(c > 0);
            }
            return response.toString();
        }
        catch(java.net.MalformedURLException ex)
        {
            System.err.println("Invalid provider url");
        }
        catch(java.io.IOException ex)
        {
            ex.printStackTrace();
        }
        return null;
    }
}