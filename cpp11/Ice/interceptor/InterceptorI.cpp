//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <algorithm>
#include <InterceptorI.h>

using namespace std;

InterceptorI::InterceptorI(shared_ptr<Ice::Object> servant, shared_ptr<AuthenticatorI> authenticator,
                           vector<string> securedOperations) :
    _servant(move(servant)), _authenticator(authenticator), _securedOperations(securedOperations)
{
}

bool
InterceptorI::dispatch(Ice::Request& request)
{
    auto current = request.getCurrent();
    //
    // Check if the operation requires authorization to invoke.
    //
    if(find(_securedOperations.begin(), _securedOperations.end(), current.operation) != _securedOperations.end())
    {
        //
        // Validate the client's access token before dispatching to the servant.
        // 'validateToken' throws an exception if the token is invalid or expired.
        //
        auto tokenValue = current.ctx.find("accessToken");
        if(tokenValue != current.ctx.end())
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
    return _servant->ice_dispatch(request);
}
