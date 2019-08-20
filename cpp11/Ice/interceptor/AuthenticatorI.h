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
    virtual std::string getToken(const Ice::Current&) override;
    void validateToken(const std::string&);

private:

    std::mt19937_64 _rand;
    std::map<std::string, long long> _tokenStore;
    std::mutex _tokenLock;
};

#endif
