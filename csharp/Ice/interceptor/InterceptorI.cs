//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System.Threading.Tasks.Task;

class InterceptorI : Ice.DispatchInterceptor
{
    public InterceptorI(Ice.Object servant, AuthenticatorI authenticator, List<String> securedOperations)
    {
        _servant = servant;
        _authenticator = authenticator;
        _securedOperations = securedOperations;
    }

    public override Task<Ice.OutputStream> dispatch(Ice.Request request)
    {
        var current = request.getCurrent();
        //
        // Check if the operation requires authorization to invoke.
        //
        if(_securedOperations.Contains(current.operation))
        {
            //
            // Validate the client's access token before dispatching to the servant.
            // 'validateToken' throws an exception if the token is invalid or expired.
            //
            string tokenValue = "";
            if(current.ctx.TryGetValue("accessToken", out tokenValue))
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

    private readonly Ice.Object _servant;
    private readonly AuthenticatorI _authenticator;
    private readonly List<String> _securedOperations;
}
