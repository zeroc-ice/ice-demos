//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef INTERCEPTOR_I_H
#define INTERCEPTOR_I_H

#include <vector>
#include <Ice/Ice.h>
#include <AuthenticatorI.h>

class InterceptorI : public Ice::DispatchInterceptor
{
    InterceptorI(std::shared_ptr<Ice::Object>, std::shared_ptr<AuthenticatorI>, std::vector<std::string>);

public:

    virtual bool dispatch(Ice::Request&) override;

private:

    std::shared_ptr<Ice::Object> _servant;
    std::shared_ptr<AuthenticatorI> _authenticator;
    std::vector<std::string> _securedOperations;
};

#endif
