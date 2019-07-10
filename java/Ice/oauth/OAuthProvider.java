//
// Copyright (c) ZeroC, Inc. All rights reserved.
//


public class OAuthProvider implements Runnable
{
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
        java.util.Map<String, String> params;

        try(java.net.Socket socket = new java.net.ServerSocket(port).accept();
            java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(socket.getInputStream()));
            java.io.BufferedWriter out = new java.io.BufferedWriter(new java.io.OutputStreamWriter(socket.getOutputStream())))
        {
            while(isRunning)
            {//This ignores all the input parameters and just spits a token out. TODO
                params = parsePOSTParameters(in.readLine());

            }
        }
        catch(java.io.IOException ex)
        {
            System.err.println("Whoops!");
            ex.printStackTrace();
        }
    }

    private java.util.Map<String, String> parsePOSTParameters(String query)
    {//TODO THIS CAN'T HANDLE WHEN THE PARAMETERS CONTAIN '&' or '='
        java.util.Map<String, String> params = new java.util.HashMap<String, String>();
        String key, value;
        int delimIndex;
        for(String param : query.split("&"))
        {
            delimIndex = param.indexOf('=');
            key = param.substring(0, delimIndex);
            value = param.substring(delimIndex + 1);
            params.put(key, value);
        }
        return params;
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
        isRunning = false;
    }
}