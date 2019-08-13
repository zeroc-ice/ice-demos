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
        _tokenStore = new List<Token>();
        _rand = new RNGCryptoServiceProvider();
    }

    public override Token getToken(string username, string password, Ice.Current current)
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

        //
        // Create a token object, store a copy, and return it.
        // By default tokens are valid for 1 hour after being issued.
        //
        Token token = new Token(new string(tokenValue), username,
                                DateTimeOffset.UtcNow.ToUnixTimeMilliseconds() + TOKEN_LIFETIME);
        lock(_tokenStore)
        {
            _tokenStore.Add((Token)token.Clone());
        }
        Console.Out.WriteLine("Issuing new access token for user: " + username + ". Token=" + token.value);
        return token;
    }

    internal void validateToken(string tokenValue)
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
