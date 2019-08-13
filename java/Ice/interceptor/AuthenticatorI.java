//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;

class AuthenticatorI implements Authenticator
{
    AuthenticatorI()
    {
        _tokenStore = new java.util.ArrayList<Token>();
        _rand = new java.security.SecureRandom();
    }

    @Override
    public Token getToken(String username, String password, com.zeroc.Ice.Current current)
    {
        //
        // Generate a random 32 character long token.
        //
        String chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        StringBuilder tokenBuilder = new StringBuilder(32);
        for(int i = 0; i < 32; i++)
        {
            tokenBuilder.append(chars.charAt(_rand.nextInt(chars.length())));
        }

        //
        // By default tokens are valid for 1 hour after being issued.
        //
        long expireTime = System.currentTimeMillis() + TOKEN_LIFETIME;

        //
        // Create a token object, store a copy, and return it.
        //
        Token token = new Token(tokenBuilder.toString(), username, expireTime);
        synchronized(_tokenStore)
        {
            _tokenStore.add(token.clone());
        }
        System.out.println("Issuing new access token for user: " + username + ". Token=" + token.value);
        return token;
    }

    void validateToken(String tokenValue)
        throws AuthorizationException
    {
        synchronized(_tokenStore)
        {
            for(Token token : _tokenStore)
            {
                //
                // Check if the authenticator has issued any tokens with a matching value.
                //
                if(token.value.equals(tokenValue))
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
        }
        throw new AuthorizationException();
    }

    public static final long TOKEN_LIFETIME = 1000 * 60 * 60;
    private final java.security.SecureRandom _rand;
    private final java.util.List<Token> _tokenStore;
}
