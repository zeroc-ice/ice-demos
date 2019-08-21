//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;

class AuthenticatorI implements Authenticator
{
    AuthenticatorI()
    {
        _tokenStore = new java.util.HashMap<>();
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
        //
        // By default tokens are valid for 30 seconds after being issued.
        //
        long expireTime = System.currentTimeMillis() + (1000 * 30);
        synchronized(_tokenStore)
        {
            _tokenStore.put(token, expireTime);
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
                long expireTime = _tokenStore.get(tokenValue);
                //
                // Delete the token if it has expired.
                //
                if(expireTime <= System.currentTimeMillis())
                {
                    _tokenStore.remove(tokenValue);
                    throw new TokenExpiredException();
                }
            }
            else
            {
                throw new AuthorizationException();
            }
        }
    }

    private final java.security.SecureRandom _rand;
    private final java.util.Map<String, Long> _tokenStore;
}
