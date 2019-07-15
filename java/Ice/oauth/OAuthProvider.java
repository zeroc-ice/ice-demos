//
// Copyright (c) ZeroC, Inc. All rights reserved.
//


public class OAuthProvider implements Runnable
{
    private java.net.ServerSocket server;
    private java.util.List<String> users;
    private java.util.List<String> accessTokens;
    private boolean isRunning;
    private final int port;

    public OAuthProvider(int port)
    {
        accessTokens = new java.util.ArrayList<String>();
        this.port = port;
        isRunning = true;
    }

    @Override
    public void run()
    {
        try
        {
            server = new java.net.ServerSocket(port);
            while(isRunning)
            {
                try
                {
                    java.net.Socket socket = server.accept();
                    synchronized(server)
                    {
                        java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(socket.getInputStream()));
                        java.io.BufferedWriter out = new java.io.BufferedWriter(new java.io.OutputStreamWriter(socket.getOutputStream()));

                        //
                        // Read in the client's request
                        //
                        StringBuilder request = new StringBuilder();
                        String requestBuffer;
                        while((requestBuffer = in.readLine()) != null)
                        {
                            request.append(requestBuffer);
                        }

                        java.util.Map<String, String> params = parseRequest(request.toString());
                        //
                        // We only validate that the grant_type is "password", as that's
                        // the only grant type this provider supports.
                        //
                        if(params.get("grant_type") != "password")
                        {
                            out.write("HTTP/1.1 400 Bad Request\n\n");
                            out.write("{\n");
                            out.write("  \"error\":\"unsupported_grant_type\"\n");
                            out.write("}\n");
                        }
                        else
                        {
                            //
                            // Generate a token and store it and it's user.
                            //
                            String token = generateToken();
                            users.add(params.get("username"));
                            accessTokens.add(token);
                            //
                            // Send the access token to the client.
                            //
                            out.write("HTTP/1.1 200 OK\n\n");
                            out.write("{\n");
                            out.write("  \"access_token\":\"" + token + "\",");
                            out.write("  \"expires_in\":86400");
                            out.write("}\n");
                        }
                    }
                }
                catch(java.net.SocketException ex)
                {
                    //
                    // This is received when shutdown is called while the server
                    // is still listening for new connections.
                    //
                    ex.printStackTrace();
                    return;
                }
                catch(java.io.IOException ex)
                {
                    ex.printStackTrace();
                }
            }
        }
        catch(java.io.IOException ex)
        {
            ex.printStackTrace();
        }
    }

    private static java.util.Map<String, String> parseRequest(String query)
    {//TODO THIS CAN'T HANDLE WHEN THE PARAMETERS CONTAIN '&' or '='
        java.util.Map<String, String> params = new java.util.HashMap<String, String>();
        System.out.println(query);
        String key, value;
        int delimIndex;
        for(String param : query.split("&"))
        {
            System.out.println(param);
            delimIndex = param.indexOf('=');
            key = param.substring(0, delimIndex);
            value = param.substring(delimIndex + 1);
            params.put(key, value);
        }
        return params;
    }

    //
    // Generates a random 32 character hex string for use as a token.
    //
    private static String generateToken()
    {
        java.util.Random rand = new java.util.Random();
        StringBuilder token = new StringBuilder();
        while(token.length() < 32)
        {
            token.append(Integer.toHexString(rand.nextInt()));
        }
        return token.toString();
    }

    public synchronized boolean checkToken(String accessToken)
    {
        if((accessToken != null) && (accessTokens.contains(accessToken)))
        {
            return true;
        }
        return false;
    }

    public void shutdown()
    {
        try
        {
            //
            // Synchronize on the server socket to ensure we don't
            // close it while it's still processing a request.
            //
            synchronized(server)
            {
                isRunning = false;
                server.close();
            }
        }
        catch(java.io.IOException ex)
        {
            System.err.println("Failed to close socket.");
            ex.printStackTrace();
        }
    }
}