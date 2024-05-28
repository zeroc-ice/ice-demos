//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef INTERCEPTOR_I_H
#define INTERCEPTOR_I_H

#include <AuthenticatorI.h>
#include <Ice/Ice.h>
#include <unordered_set>

class InterceptorI : public Ice::Object
{
public:
    InterceptorI(std::shared_ptr<Ice::Object>, std::shared_ptr<AuthenticatorI>, std::unordered_set<std::string>);
    void dispatch(Ice::IncomingRequest&, std::function<void(Ice::OutgoingResponse)>) final;

private:
    std::shared_ptr<Ice::Object> _next;
    std::shared_ptr<AuthenticatorI> _authenticator;
    std::unordered_set<std::string> _securedOperations;
};

#endif
