//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;
using System.Collections.Generic;

public AuthenticatorI : AuthenticatorDisp_
{
    public AuthenticatorI()
    {
        _tokenStore = new List<String>();
    }

    public override Token getToken(string username, string password, Ice.Current current)
    {
        //
        // Generate a random 32 character long token.
        //
        Random rand = new Random();
        string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
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
        lock(_tokenLock)
        {
            _tokenStore.Add(token);//TODO MAKE THIS A CLONE!
        }
        Console.Out.WriteLine("Issuing new access token for user: " + username + ". Token=" + token.id);
        return token;
    }

    public void validateToken(string tokenId)
    {
        lock(_tokenLock)
        {
            foreach(var token in _tokenStore)
            {
                //
                // Check if the authenticator has issued any tokens with a matching Id.
                //
                if(token.id == tokenId)
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
    private readonly List<string> _tokenStore();
    private readonly object _tokenLock = new Object();
}
