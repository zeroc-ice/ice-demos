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
    public Token getToken(String username, String password, com.zeroc.Ice.Current current)
    {
        //
        // Generate a random 32 character long token.
        //
        java.util.Random rand = new java.util.Random();
        String chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
        StringBuilder tokenBuilder = new StringBuilder(32);
        for(int i = 0; i < 32; i++)
        {
            tokenBuilder.append(chars.charAt(rand.nextInt(chars.length())));
        }

        //
        // By default tokens are valid for 1 hour after being issued.
        //
        long expireTime = System.currentTimeMillis() + TOKEN_LIFETIME;

        //
        // Create a token object, store a copy, and return it.
        //
        Token token = new Token(tokenBuilder.toString(), username, expireTime);
        synchronized(_tokenLock)
        {
            _tokenStore.add(token.clone());
        }
        System.out.println("Issuing new access token for user: " + username + ". Token=" + token.id);
        return token;
    }

    @Override
    public void validateToken(String tokenId, com.zeroc.Ice.Current current)
        throws AuthorizationException
    {
        synchronized(_tokenLock)
        {
            for(Token token : _tokenStore)
            {
                //
                // Check if the authenticator has issued any tokens with a matching Id.
                //
                if(token.id.equals(tokenId))
                {
                    //
                    // Delete the token if it has expired.
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
    }

    public static final long TOKEN_LIFETIME = 1000 * 60 * 60;
    private final java.util.List<Token> _tokenStore;
    private final Object _tokenLock = new Object();
}
