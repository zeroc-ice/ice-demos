//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "InterceptorI.h"

using namespace std;

InterceptorI::InterceptorI(
    shared_ptr<Ice::Object> next,
    shared_ptr<AuthenticatorI> authenticator,
    unordered_set<string> securedOperations)
    : _next(std::move(next)),
      _authenticator(std::move(authenticator)),
      _securedOperations(std::move(securedOperations))
{
}

void
InterceptorI::dispatch(Ice::IncomingRequest& request, function<void(Ice::OutgoingResponse)> sendResponse)
{
    auto current = request.current();
    //
    // Check if the operation requires authorization to invoke.
    //
    if (_securedOperations.find(current.operation) != _securedOperations.end())
    {
        //
        // Validate the client's access token before dispatching to the servant.
        // 'validateToken' throws an exception if the token is invalid or expired.
        //
        auto tokenValue = current.ctx.find("accessToken");
        if (tokenValue != current.ctx.end())
        {
            _authenticator->validateToken(tokenValue->second);
        }
        else
        {
            //
            // If the client didn't include an accessToken, throw an exception.
            //
            throw Demo::AuthorizationException();
        }
    }
    _next->dispatch(request, sendResponse);
}
