//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;
import com.zeroc.Ice.Current;
import java.security.SecureRandom;
import java.util.Iterator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

class AuthenticatorI implements Authenticator
{
    AuthenticatorI()
    {
        _tokenStore = new HashMap<>();
        _rand = new SecureRandom();
    }

    @Override
    public String getToken(Current current)
    {
        //
        // Generate a random 32 character long token.
        //
        String chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        char[] tokenValue = new char[32];
        byte[] bytes = new byte[tokenValue.length];
        _rand.nextBytes(bytes);

        for(int i = 0; i < bytes.length; ++i)
        {
            //
            // Since bytes are signed in java, we need to shift their values
            // up by 128 (Byte.MIN_VALUE) before using their remainder as indexes.
            //
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
        return token;
    }

    void validateToken(String tokenValue)
        throws AuthorizationException
    {
        synchronized(_tokenStore)
        {
            //
            // Remove any expired tokens.
            //
            for(String token : new HashSet<String>(_tokenStore.keySet()))
            {
                if(_tokenStore.get(token) <= System.currentTimeMillis())
                {
                    _tokenStore.remove(token);
                }
            }

            //
            // We assume if the client passed a token, but there's no
            // stored values matching it, that it must of expired.
            //
            if(!_tokenStore.containsKey(tokenValue))
            {
                throw new TokenExpiredException();
            }
        }
    }

    private final SecureRandom _rand;
    private final Map<String, Long> _tokenStore;
}
