//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <chrono>
#include <iostream>
#include <Ice/Ice.h>
#include <AuthenticatorI.h>

using namespace std;

long long int getCurrentTimeMillis()
{
    auto duration = chrono::system_clock::now().time_since_epoch();
    return chrono::duration_cast<chrono::milliseconds>(duration).count();
}

AuthenticatorI::AuthenticatorI() :
    _rand(getCurrentTimeMillis())
{
}

Demo::Token
AuthenticatorI::getToken(string username, string password, const Ice::Current&)
{
    //
    // Generate a random 32 character long token.
    //
    string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    uniform_int_distribution<int> dist(0, chars.length());
    char tokenBuilder[32];
    for(auto i = 0; i < 32; i++)
    {
        tokenBuilder[i] = chars.at(dist(_rand));
    }

    //
    // Create a token object, store a copy, and return it.
    //
    Demo::Token token;
    string tokenValue(tokenBuilder);
    token.value = tokenValue;
    token.username = username;
    //
    // By default tokens are valid for 1 hour after being issued.
    //
    token.expireTime = getCurrentTimeMillis() + TOKEN_LIFETIME;
    {
        lock_guard<mutex> lock(_tokenLock);
        _tokenStore.push_back(token);
    }
    cout << "Issuing new access token for user: " + username + ". Token=" + tokenValue << endl;
    return token;
}

void
AuthenticatorI::validateToken(const string& tokenValue)
{
    lock_guard<mutex> lock(_tokenLock);

    for(unsigned int i = 0; i < _tokenStore.size(); i++)
    {
        auto token = _tokenStore[i];
        //
        // Check if the authenticator has issued any tokens with a matching value.
        //
        if(token.value == tokenValue)
        {
            //
            // Delete the token if it has expired.
            //
            if(token.expireTime <= getCurrentTimeMillis())
            {
                _tokenStore.erase(_tokenStore.begin() + i);
                throw Demo::TokenExpiredException();
            }
            else
            {
                return;
            }
        }
    }
    throw Demo::AuthorizationException();
}
