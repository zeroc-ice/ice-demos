// Copyright (c) ZeroC, Inc.

import com.zeroc.demos.Ice.interceptor.Demo.*;
import com.zeroc.Ice.Current;
import com.zeroc.Ice.IncomingRequest;
import com.zeroc.Ice.OutgoingResponse;
import java.util.concurrent.CompletionStage;
import java.util.Set;

class InterceptorI implements com.zeroc.Ice.Object
{
    InterceptorI(com.zeroc.Ice.Object servant, AuthenticatorI authenticator, Set<String> securedOperations)
    {
        _servant = servant;
        _authenticator = authenticator;
        _securedOperations = securedOperations;
    }

    @Override
    public CompletionStage<OutgoingResponse> dispatch(IncomingRequest request)
        throws com.zeroc.Ice.UserException
    {
        //
        // Check if the operation requires authorization to invoke.
        //
        if(_securedOperations.contains(request.current.operation))
        {
            //
            // Validate the client's access token before dispatching to the servant.
            // 'validateToken' throws an exception if the token is invalid or expired.
            //
            String tokenValue = request.current.ctx.get("accessToken");
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
        return _servant.dispatch(request);
    }

    private final com.zeroc.Ice.Object _servant;
    private final AuthenticatorI _authenticator;
    private final Set<String> _securedOperations;
}
