// Copyright (c) ZeroC, Inc.

using Demo;
using System.Security.Cryptography;

internal class AuthenticatorI : AuthenticatorDisp_
{
    internal AuthenticatorI()
    {
        _tokenStore = new Dictionary<string, DateTimeOffset>();
        _rand = RandomNumberGenerator.Create();
    }

    public override string getToken(Ice.Current current)
    {
        // Generate a random 32 character long token.
        string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        char[] tokenValue = new char[32];
        byte[] bytes = new byte[tokenValue.Length];
        _rand.GetBytes(bytes);

        for (var i = 0; i < bytes.Length; ++i)
        {
            tokenValue[i] = chars[bytes[i] % chars.Length];
        }

        string token = new string(tokenValue);
        // By default tokens are valid for 30 seconds after being issued.
        DateTimeOffset expireTime = DateTimeOffset.UtcNow.AddSeconds(30);
        lock (_tokenStore)
        {
            _tokenStore.Add(token, expireTime);
        }
        return token;
    }

    internal void ValidateToken(string tokenValue)
    {
        lock (_tokenStore)
        {
            // Remove any expired tokens.
            foreach (string token in new List<string>(_tokenStore.Keys))
            {
                if (_tokenStore[token] <= DateTimeOffset.UtcNow)
                {
                    _tokenStore.Remove(token);
                }
            }

            // We assume if the client passed a token, but there's no
            // stored values matching it, that it must of expired.
            if (!_tokenStore.ContainsKey(tokenValue))
            {
                throw new TokenExpiredException();
            }
        }
    }

    private readonly RandomNumberGenerator _rand;
    private readonly Dictionary<string, DateTimeOffset> _tokenStore;
}
