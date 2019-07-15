//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.sun.net.httpserver.Headers;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

public class OAuthProvider implements HttpHandler, Runnable
{
    private final HttpServer server;
    private java.util.List<String> accessTokens;
    private java.util.List<String> users;
    
    public OAuthProvider(int port) throws java.io.IOException
    {
        server = HttpServer.create(new java.net.InetSocketAddress(port), 0);
        accessTokens = new java.util.ArrayList<String>();
        users = new java.util.ArrayList<String>();
    }

    @Override
    public void run()
    {
        //
        // Set this class to be the handler for all incoming requests,
        // and then start the server.
        //
        server.createContext("/", this);
        server.start();
    }

    @Override
    public void handle(HttpExchange exchange)
    {//TODO check encoding and pass it along to the reader/writer
        try(java.io.InputStreamReader in = new java.io.InputStreamReader(exchange.getRequestBody(), "UTF-8");
            java.io.OutputStream out = exchange.getResponseBody())
        {
            //
            // Read the client's request.
            //
            Headers headers = exchange.getRequestHeaders();
            int requestLength = Integer.parseInt(headers.getFirst("Content-Length"));
            char[] buffer = new char[requestLength];
            if(in.read(buffer) != requestLength)
            {
                throw new java.io.IOException("Reached EOS while parsing request body.");
            }
            String request = new String(buffer);
            java.util.Map<String, String> params = parseRequest(request);

            //
            // We only validate that the grant_type is "password" (as that's the only
            // grant type this provider supports) and nothing else.
            //
            StringBuilder response = new StringBuilder();
            int rCode = 200;
            if(params.get("grant_type").equals("password"))
            {
                //
                // Generate and store a token along with it's user.
                //
                String token = generateToken();
                String user = params.get("username");
                users.add(user);
                accessTokens.add(token);

                //
                // Send the access token to the client. This also sends an 'expires_in'
                // field for the client to use, even though tokens never actually expire.
                //
                response.append("{\r\n");
                response.append("  \"access_token\":\"" + token + "\",\r\n");
                response.append("  \"expires_in\":86400\r\n");
                response.append("}");

                System.out.println("Issuing new access token for user: " + user + ". Token=\"" + token + "\"");
            }
            else
            {
                response.append("{\r\n");
                response.append("  \"error\":\"unsupported_grant_type\"\r\n");
                response.append("}");
                // HTTP error code for BAD_REQUEST
                rCode = 400;
            }

            //
            // Send the response to the client.
            //
            byte[] data = response.toString().getBytes("UTF-8");
            exchange.sendResponseHeaders(rCode, data.length);
            out.write(data);
        }
        catch(java.io.IOException ex)
        {
            System.err.println("Error while processing request");
            ex.printStackTrace();
        }
    }

    private static java.util.Map<String, String> parseRequest(String query)
        throws java.io.UnsupportedEncodingException
    {
        java.util.Map<String, String> params = new java.util.HashMap<String, String>();
        String key, value;
        int delimIndex;
        for(String param : query.split("&"))
        {
            delimIndex = param.indexOf('=');
            key = java.net.URLDecoder.decode(param.substring(0, delimIndex), "UTF-8");
            value = java.net.URLDecoder.decode(param.substring(delimIndex + 1), "UTF-8");
            params.put(key, value);
        }
        return params;
    }

    public void shutdown()
    {
        //
        // Waits up to 5 seconds for the server to finish processing
        // requests before closing it's connections and shutting down.
        //
        server.stop(5);
    }

    private static String generateToken()
    {
        //
        // Generates a random 32 character hex string for use as a token.
        //
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
}
