// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Collections.Generic;
using System.Security.Cryptography;
using ZeroC.Ice;

namespace Demo
{
    class Authenticator : IAuthenticator
    {
        private readonly RNGCryptoServiceProvider _rand;
        private readonly Dictionary<string, DateTimeOffset> _tokenStore;

        internal Authenticator()
        {
            _tokenStore = new Dictionary<string, DateTimeOffset>();
            _rand = new RNGCryptoServiceProvider();
        }

        public string GetToken(Current current)
        {
            //
            // Generate a random 32 character long token.
            //
            string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            char[] tokenValue = new char[32];
            byte[] bytes = new byte[tokenValue.Length];
            _rand.GetBytes(bytes);

            for (int i = 0; i < bytes.Length; ++i)
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
                foreach (string token in _tokenStore.Keys)
                {
                    if (_tokenStore[token] <= DateTimeOffset.UtcNow)
                    {
                        _tokenStore.Remove(token);
                    }
                }

                // We assume if the client passed a token, but there's no stored values matching it, that it must of
                // expired.
                if (!_tokenStore.ContainsKey(tokenValue))
                {
                    throw new TokenExpiredException();
                }
            }
        }
    }
}
