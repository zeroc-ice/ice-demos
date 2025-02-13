// Copyright (c) ZeroC, Inc.

namespace Server;

/// <summary>The AuthorizationMiddleware intercepts incoming requests and checks the token entry in their contexts.
/// </summary>
internal class AuthorizationMiddleware : Ice.Object
{
    private readonly Ice.Object _next;
    private readonly string _validToken;

    /// <inheritdoc />
    public ValueTask<Ice.OutgoingResponse> dispatchAsync(Ice.IncomingRequest request)
    {
        // Check if the request has a valid token in its context.
        if (!request.current.ctx.TryGetValue("token", out string? token) || token != _validToken)
        {
            Console.WriteLine($"Rejecting request with token '{token}'");

            // ObjectNotExistException is one of the 6 Ice local exceptions that Ice transmits "over the wire".
            // It usually means "no servant was found for this identity". Here, we reuse it to indicate an authorization
            // failure without leaking information to the client.
            throw new Ice.ObjectNotExistException();
        }

        Console.WriteLine($"Accepting request with token '{token}'");

        // Continue the dispatch pipeline.
        return _next.dispatchAsync(request);
    }

    /// <summary>Constructs an AuthorizationMiddleware.</summary>
    /// <param name="next">The next Ice.Object in the dispatch pipeline.</param>
    /// <param name="validToken">The value to check against.</param>
    internal AuthorizationMiddleware(Ice.Object next, string validToken)
    {
        _next = next;
        _validToken = validToken;
    }
}
