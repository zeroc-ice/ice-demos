//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef AUTHENTICATOR_I_H
#define AUTHENTICATOR_I_H

#include <mutex>
#include <random>
#include <map>
#include <Interceptor.h>

class AuthenticatorI : public Demo::Authenticator
{
public:

    AuthenticatorI();
    virtual Demo::Token getToken(std::string, std::string, const Ice::Current&) override;
    void validateToken(const std::string&);

    static const long long TOKEN_LIFETIME = 1000 * 60 * 60;

private:

    std::mt19937_64 _rand;
    std::map<std::string, Demo::Token> _tokenStore;
    std::mutex _tokenLock;
};

#endif
