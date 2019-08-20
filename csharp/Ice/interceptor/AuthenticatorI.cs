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
        _tokenStore = new Dictionary<string, long>();
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
        long expireTime = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds() + (1000 * 60 * 60);
        lock(_tokenStore)
        {
            _tokenStore.Add(token, expireTime);
        }
        Console.Out.WriteLine("Issuing new access token. Token=" + token);
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
            }
            else
            {
                throw new AuthorizationException();
            }
        }
    }

    private readonly RNGCryptoServiceProvider _rand;
    private readonly Dictionary<string, long> _tokenStore;
}
