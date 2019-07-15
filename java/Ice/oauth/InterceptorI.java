//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import java.util.concurrent.CompletionStage;
import com.zeroc.demos.Ice.oauth.Demo.*;

public class InterceptorI extends com.zeroc.Ice.DispatchInterceptor
{
    private final java.util.List<String> securedOperations;
    private final com.zeroc.Ice.Object servant;
    private final OAuthProvider provider;

    public InterceptorI(com.zeroc.Ice.Object servant, OAuthProvider provider)
    {
        this.servant = servant;
        this.provider = provider;

        securedOperations = new java.util.ArrayList<String>();
        //
        // We only require authorization for the 'setTemp' operation.
        //
        securedOperations.add("setTemp");
    }

    @Override
    public CompletionStage<com.zeroc.Ice.OutputStream> dispatch(com.zeroc.Ice.Request request)
        throws com.zeroc.Ice.UserException
    {
        com.zeroc.Ice.Current current = request.getCurrent();
        //TODO
        current.encoding = new com.zeroc.Ice.EncodingVersion((byte)1, (byte)1);
        //
        // Check if the operation requires authorization to invoke.
        //
        if(securedOperations.contains(current.operation))
        {
            //
            // Validate the client's access token before dispatching to the servant.
            //
            if(!provider.checkToken(current.ctx.get("accessToken")))
            {
                throw new AuthorizationException();
            }
        }
        return servant.ice_dispatch(request);
    }
}
