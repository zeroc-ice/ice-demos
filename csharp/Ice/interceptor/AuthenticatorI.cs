//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;
using System.Collections.Generic;
using System.Security.Cryptography.RNGCryptoServiceProvider;

class AuthenticatorI : AuthenticatorDisp_
{
    public AuthenticatorI()
    {
        _tokenStore = new List<String>();
        _rand = new RNGCryptoServiceProvider();
    }

    public override Token getToken(string username, string password, Ice.Current current)
    {
        //
        // Generate a random 32 character long token.
        //
        RNGCryptoServiceProvider rand = new RNGCryptoServiceProvider();
        string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        StringBuilder tokenBuilder = new StringBuilder(32);
        for(var i = 0; i < 32; i++)
        {
            tokenBuilder.Append(chars[rand.Next(chars.Length)]);
        }

        //
        // By default tokens are valid for 1 hour after being issued.
        //
        long expireTime = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds() + TOKEN_LIFETIME;

        //
        // Create a token object, store a copy, and return it.
        //
        Token token = new Token(tokenBuilder.ToString(), username, expireTime);
        lock(_tokenStore)
        {
            _tokenStore.Add(token.Clone());
        }
        Console.Out.WriteLine("Issuing new access token for user: " + username + ". Token=" + token.value);
        return token;
    }

    public void validateToken(string tokenValue)
    {
        lock(_tokenStore)
        {
            foreach(var token in _tokenStore)
            {
                //
                // Check if the authenticator has issued any tokens with a matching value.
                //
                if(token.value == tokenValue)
                {
                    //
                    // Delete the token if it has expired.
                    //
                    if(token.expireTime <= DateTimeOffset.UtcNow.ToUnixTimeMilliseconds())
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

    public static const long TOKEN_LIFETIME = 1000 * 60 * 60;
    private readonly RNGCryptoServiceProvider _rand;
    private readonly List<string> _tokenStore;
}
