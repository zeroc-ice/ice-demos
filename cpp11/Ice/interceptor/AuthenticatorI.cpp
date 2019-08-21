//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <iostream>
#include <Ice/Ice.h>
#include <AuthenticatorI.h>

using namespace std;

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
    // By default tokens are valid for 30 seconds after being issued.
    //
    auto expireTime = chrono::steady_clock::now() + chrono::seconds(30);
    {
        lock_guard<mutex> lock(_tokenLock);
        _tokenStore.insert(pair<string, chrono::time_point<std::chrono::steady_clock>>(token, expireTime));
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
        if(token->second <= chrono::steady_clock::now())
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
