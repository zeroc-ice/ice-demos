//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <chrono>
#include <iostream>
#include <Ice/Ice.h>
#include <AuthenticatorI.h>

using namespace std;

long long getCurrentTimeMillis()
{
    auto duration = chrono::system_clock::now().time_since_epoch();
    return chrono::duration_cast<chrono::milliseconds>(duration).count();
}

//
// Use a value generated from a random device to seed the RNG.
//
AuthenticatorI::AuthenticatorI() :
    _rand(random_device()())
{
}

string
AuthenticatorI::getToken(const Ice::Current&)
{
    //
    // Generate a random 32 character long token.
    //
    const int tokenLength = 32;
    string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    uniform_int_distribution<int> dist(0, static_cast<int>(chars.length() - 1));
    char tokenBuilder[tokenLength];
    for(auto i = 0; i < tokenLength; i++)
    {
        tokenBuilder[i] = chars.at(dist(_rand));
    }

    string token(tokenBuilder, tokenLength);
    //
    // By default tokens are valid for 1 hour after being issued.
    //
    long long expireTime = getCurrentTimeMillis() + (1000 * 60 * 60);
    {
        lock_guard<mutex> lock(_tokenLock);
        _tokenStore.insert(pair<string, long long>(token, expireTime));
    }
    cout << "Issuing new access token. Token=" << token << endl;
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
        if(token->second <= getCurrentTimeMillis())
        {
            _tokenStore.erase(token);
            throw Demo::TokenExpiredException();
        }
    }
    else
    {
        throw Demo::AuthorizationException();
    }
}
