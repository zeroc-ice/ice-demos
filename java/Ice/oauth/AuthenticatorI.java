//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;

public class AuthenticatorI implements Authenticator
{
    public AuthenticatorI()
    {
        _tokenStore = new java.util.ArrayList<Token>();
    }

    @Override
    public synchronized Token getToken(String username, String password, com.zeroc.Ice.Current current)
    {
        //
        // Generate a random 32 character long hex string to use as a token.
        //
        java.util.Random rand = new java.util.Random();
        StringBuilder tokenBuilder = new StringBuilder();
        while(tokenBuilder.length() < 32)
        {
            tokenBuilder.append(Integer.toHexString(rand.nextInt()));
        }

        //
        // By default tokens are valid for 1 hour after being issued.
        //
        long expireTime = System.currentTimeMillis() + tokenLifetime;

        //
        // Create a token object, store a copy, and return it.
        //
        Token token = new Token(tokenBuilder.toString(), username, expireTime);
        _tokenStore.add(token.clone());
        System.out.println("Issuing new access token for user: " + username + ". Token=" + token.id);
        return token;
    }

    @Override
    public synchronized void validateToken(String tokenId, com.zeroc.Ice.Current current)
        throws AuthorizationException
    {
        for(Token token : _tokenStore)
        {
            //
            // Check if the authenticator has issued any tokens with a matching Id.
            //
            if(token.id.equals(tokenId))
            {
                //
                // Delete the token if it has expired, otherwise return true.
                //
                if(token.expireTime <= System.currentTimeMillis())
                {
                    _tokenStore.remove(token);
                    throw new TokenExpiredException();
                }
                else
                {
                    return;
                }
            }
        }
        throw new AuthorizationException();
    }

    public static final long tokenLifetime = 1000 * 60 * 60;
    private final java.util.List<Token> _tokenStore;
}
