//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import java.util.concurrent.CompletionStage;
import com.zeroc.demos.Ice.interceptor.Demo.*;

public class InterceptorI extends com.zeroc.Ice.DispatchInterceptor
{
    public InterceptorI(com.zeroc.Ice.Object servant, AuthenticatorI authenticator)
    {
        _servant = servant;
        _authenticator = authenticator;
        _securedOperations = new java.util.ArrayList<String>();

        //
        // We only require authorization for the 'setTemp' operation.
        //
        _securedOperations.add("setTemp");
    }

    @Override
    public CompletionStage<com.zeroc.Ice.OutputStream> dispatch(com.zeroc.Ice.Request request)
        throws com.zeroc.Ice.UserException
    {
        com.zeroc.Ice.Current current = request.getCurrent();
        //
        // Check if the operation requires authorization to invoke.
        //
        if(_securedOperations.contains(current.operation))
        {
            //
            // Validate the client's access token before dispatching to the servant.
            // 'validateToken' throws an exception if the token is invalid or expired.
            //
            String tokenId = current.ctx.get("accessToken");
            if(tokenId != null)
            {
                _authenticator.validateToken(tokenId);
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

    private final java.util.List<String> _securedOperations;
    private final com.zeroc.Ice.Object _servant;
    private final AuthenticatorI _authenticator;
}
