//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;

class AuthenticatorI implements Authenticator
{
    AuthenticatorI()
    {
        _tokenStore = new java.util.HashMap<String, Token>();
        _rand = new java.security.SecureRandom();
    }

    @Override
    public Token getToken(String username, String password, com.zeroc.Ice.Current current)
    {
        //
        // Generate a random 32 character long token.
        //
        String chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        char[] tokenValue = new char[32];
        byte[] bytes = new byte[tokenValue.length];
        _rand.nextBytes(bytes);

        for(int i = 0; i < bytes.length; i++)
        {
            tokenValue[i] = chars.charAt((bytes[i] + 128) % chars.length());
        }

        //
        // Create a token object, store a copy, and return it.
        // By default tokens are valid for 1 hour after being issued.
        //
        Token token = new Token(new String(tokenValue), username,
                                System.currentTimeMillis() + TOKEN_LIFETIME);
        synchronized(_tokenStore)
        {
            _tokenStore.put(token.value, token.clone());
        }
        System.out.println("Issuing new access token for user: " + username + ". Token=" + token.value);
        return token;
    }

    void validateToken(String tokenValue)
        throws AuthorizationException
    {
        synchronized(_tokenStore)
        {
            //
            // Check if the authenticator has issued any tokens with a matching value.
            //
            Token token = _tokenStore.get(tokenValue);
            if(token != null)
            {
                //
                // Delete the token if it has expired.
                //
                if(token.expireTime <= System.currentTimeMillis())
                {
                    _tokenStore.remove(tokenValue);
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

    public static final long TOKEN_LIFETIME = 1000 * 60 * 60;
    private final java.security.SecureRandom _rand;
    private final java.util.Map<String, Token> _tokenStore;
}
