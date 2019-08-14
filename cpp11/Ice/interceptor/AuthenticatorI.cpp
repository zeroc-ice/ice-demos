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
    uniform_int_distribution<int> dist(0, static_cast<int>(chars.length()));
    char tokenBuilder[32];
    for(auto i = 0; i < 32; i++)
    {
        tokenBuilder[i] = chars.at(dist(_rand));
    }

    //
    // Create a token object, store a copy, and return it.
    //
    Demo::Token token;
    token.value = string(tokenBuilder);
    token.username = username;
    //
    // By default tokens are valid for 1 hour after being issued.
    //
    token.expireTime = getCurrentTimeMillis() + TOKEN_LIFETIME;
    {
        lock_guard<mutex> lock(_tokenLock);
        _tokenStore.insert(pair<string, Demo::Token>(token.value, token));
    }
    cout << "Issuing new access token for user: " + username + ". Token=" + token.value << endl;
    return token;
}

void
AuthenticatorI::validateToken(const string& tokenValue)
{
    lock_guard<mutex> lock(_tokenLock);

    //
    // Check if the authenticator has issued any tokens with a matching value.
    //
    auto token = _tokenStore.find(tokenValue);
    if(token != _tokenStore.end())
    {
        //
        // Delete the token if it has expired.
        //
        if(token->second.expireTime <= getCurrentTimeMillis())
        {
            _tokenStore.erase(token);
            throw Demo::TokenExpiredException();
        }
        else
        {
            return;
        }
    }
    throw Demo::AuthorizationException();
}
