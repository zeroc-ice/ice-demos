//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;

class AuthenticatorI implements Authenticator
{
    AuthenticatorI()
    {
        _tokenStore = new java.util.HashMap<String, Long>();
        _rand = new java.security.SecureRandom();
    }

    @Override
    public String getToken(com.zeroc.Ice.Current current)
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

        String token = new String(tokenValue);
        synchronized(_tokenStore)
        {
            //
            // By default tokens are valid for 1 hour after being issued.
            //
            _tokenStore.put(token, System.currentTimeMillis() + TOKEN_LIFETIME);
        }
        System.out.println("Issuing new access token. Token=" + token);
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
            if(_tokenStore.containsKey(tokenValue))
            {
                //
                // Delete the token if it has expired.
                //
                if(_tokenStore.get(tokenValue) <= System.currentTimeMillis())
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
    private final java.util.Map<String, Long> _tokenStore;
}
