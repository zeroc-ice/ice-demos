//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Text;

class AuthenticatorI : AuthenticatorDisp_
{
    internal AuthenticatorI()
    {
        _tokenStore = new Dictionary<String, long>();
        _rand = new RNGCryptoServiceProvider();
    }

    public override string getToken(Ice.Current current)
    {
        //
        // Generate a random 32 character long token.
        //
        string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        char[] tokenValue = new char[32];
        byte[] bytes = new byte[tokenValue.Length];
        _rand.GetBytes(bytes);

        for(var i = 0; i < bytes.Length; i++)
        {
            tokenValue[i] = chars[bytes[i] % chars.Length];
        }

        string token = new string(tokenValue);
        //
        // By default tokens are valid for 1 hour after being issued.
        //
        long expireTime = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds() + TOKEN_LIFETIME;
        lock(_tokenStore)
        {
            _tokenStore.Add(token, expireTime);
        }
        Console.Out.WriteLine("Issuing new access token for user: " + username + ". Token=" + token.value);
        return token;
    }

    internal void validateToken(string tokenValue)
    {
        lock(_tokenStore)
        {
            //
            // Check if the authenticator has issued any tokens with a matching value.
            //
            long expireTime;
            if(_tokenStore.TryGetValue(tokenValue, out expireTime))
            {
                //
                // Delete the token if it has expired.
                //
                if(expireTime <= DateTimeOffset.UtcNow.ToUnixTimeMilliseconds())
                {
                    _tokenStore.Remove(tokenValue);
                    throw new TokenExpiredException();
                }
                else
                {
                    return;
                }
            }
            throw new AuthorizationException();
        }
    }

    public const long TOKEN_LIFETIME = 1000 * 60 * 60;
    private readonly RNGCryptoServiceProvider _rand;
    private readonly Dictionary<String, long> _tokenStore;
}
