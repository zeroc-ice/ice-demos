// Copyright (c) ZeroC, Inc.

using Demo;

internal class InterceptorI : Ice.Object
{
    internal InterceptorI(Ice.Object servant, AuthenticatorI authenticator, HashSet<string> securedOperations)
    {
        _servant = servant;
        _authenticator = authenticator;
        _securedOperations = securedOperations;
    }

    public ValueTask<Ice.OutgoingResponse> dispatchAsync(Ice.IncomingRequest request)
    {
        var current = request.current;
        // Check if the operation requires authorization to invoke.
        if (_securedOperations.Contains(current.operation))
        {
            // Validate the client's access token before dispatching to the servant.
            // 'validateToken' throws an exception if the token is invalid or expired.
            string tokenValue;
            if (current.ctx.TryGetValue("accessToken", out tokenValue))
            {
                _authenticator.ValidateToken(tokenValue);
            }
            else
            {
                // If the client didn't include an accessToken, throw an exception.
                throw new AuthorizationException();
            }
        }
        return _servant.dispatchAsync(request);
    }

    private readonly Ice.Object _servant;
    private readonly AuthenticatorI _authenticator;
    private readonly HashSet<string> _securedOperations;
}
