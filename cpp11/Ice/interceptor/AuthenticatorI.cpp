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
    lock_guard<mutex> lock(_tokenLock);
    //
    // Generate a random 32 character long token.
    //
    const int tokenLength = 32;
    static string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    uniform_int_distribution<int> dist(0, static_cast<int>(chars.length() - 1));
    string token(tokenLength, '\0');
    for(auto& c : token)
    {
        c = chars.at(dist(_rand));
    }

    //
    // By default tokens are valid for 30 seconds after being issued.
    //
    auto expireTime = chrono::steady_clock::now() + chrono::seconds(30);
    _tokenStore.insert(pair<string, chrono::time_point<std::chrono::steady_clock>>(token, expireTime));

    cout << "Issuing new access token. Token=" << token << endl;
    return token;
}

void
AuthenticatorI::validateToken(const string& tokenValue)
{
    lock_guard<mutex> lock(_tokenLock);

    //
    // Remove any expired tokens.
    //
    for(auto token = _tokenStore.begin(); token != _tokenStore.end(); token++)
    {
        if(token->second <= chrono::steady_clock::now())
        {
            token = _tokenStore.erase(token);
        }
    }

    //
    // We assume if the client passed a token, but there's no
    // stored values matching it, that it must of expired.
    //
    if(_tokenStore.find(tokenValue) == _tokenStore.end())
    {
        throw Demo::TokenExpiredException();
    }
}
