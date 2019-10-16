//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;
import com.zeroc.Ice.Current;
import com.zeroc.Ice.Request;
import java.util.concurrent.CompletionStage;
import java.util.Set;

class InterceptorI extends com.zeroc.Ice.DispatchInterceptor
{
    InterceptorI(com.zeroc.Ice.Object servant, AuthenticatorI authenticator, Set<String> securedOperations)
    {
        _servant = servant;
        _authenticator = authenticator;
        _securedOperations = securedOperations;
    }

    @Override
    public CompletionStage<com.zeroc.Ice.OutputStream> dispatch(Request request)
        throws com.zeroc.Ice.UserException
    {
        Current current = request.getCurrent();
        //
        // Check if the operation requires authorization to invoke.
        //
        if(_securedOperations.contains(current.operation))
        {
            //
            // Validate the client's access token before dispatching to the servant.
            // 'validateToken' throws an exception if the token is invalid or expired.
            //
            String tokenValue = current.ctx.get("accessToken");
            if(tokenValue != null)
            {
                _authenticator.validateToken(tokenValue);
            }
            else
            {
                //
                // If the client didn't include an accessToken, throw an exception.
                //
                throw new AuthorizationException();
            }
        }
        return _servant.ice_dispatch(request);
    }

    private final com.zeroc.Ice.Object _servant;
    private final AuthenticatorI _authenticator;
    private final Set<String> _securedOperations;
}
