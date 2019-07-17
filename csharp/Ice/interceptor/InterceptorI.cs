//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System.Threading.Tasks.Task;

public class InterceptorI : Ice.DispatchInterceptor
{
    public InterceptorI(Ice.Object servant, AuthenticatorI authenticator)
    {
        _servant = servant;
        _authenticator = authenticator;
        _securedOperations = new List<String>();

        //
        // We only require authorization for the 'setTemp' operation.
        //
        _securedOperations.Add("setTemp");
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
            string tokenId = "";
            if(current.ctx.TryGetValue("accessToken", out tokenId))
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

    private readonly List<String> _securedOperations;
    private readonly Ice.Object _servant;
    private readonly AuthenticatorI _authenticator;
}
