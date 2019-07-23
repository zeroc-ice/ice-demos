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
    public AuthenticatorI()
    {
        _tokenStore = new List<Token>();
        _rand = new RNGCryptoServiceProvider();
    }

    public override Token getToken(string username, string password, Ice.Current current)
    {
        // Byte array for holding randomly generated value.
        byte[] nextRandom = new byte[1];
        //
        // Generate a random 32 character long token.
        //
        RNGCryptoServiceProvider rand = new RNGCryptoServiceProvider();
        string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        StringBuilder tokenBuilder = new StringBuilder(32);
        for(var i = 0; i < 32; i++)
        {
            do {
                rand.GetBytes(nextRandom);
            } while(nextRandom[0] >= chars.Length);

            tokenBuilder.Append(chars[nextRandom[0]]);
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
            _tokenStore.Add((Token)token.Clone());
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
                        _tokenStore.Remove(token);
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

    public const long TOKEN_LIFETIME = 1000 * 60 * 60;
    private readonly RNGCryptoServiceProvider _rand;
    private readonly List<Token> _tokenStore;
}
