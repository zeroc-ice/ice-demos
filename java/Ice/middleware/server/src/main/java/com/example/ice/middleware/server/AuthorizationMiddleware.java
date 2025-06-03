// Copyright (c) ZeroC, Inc.

package com.example.ice.middleware.server;

import com.zeroc.Ice.DispatchException;
import com.zeroc.Ice.IncomingRequest;
import com.zeroc.Ice.OutgoingResponse;
import com.zeroc.Ice.ReplyStatus;
import com.zeroc.Ice.UserException;
import java.util.concurrent.CompletionStage;

/**
 * The AuthorizationMiddleware intercepts incoming requests and checks the token entry in their contexts.
 */
class AuthorizationMiddleware implements com.zeroc.Ice.Object {
    private final com.zeroc.Ice.Object _next;
    private final String _validToken;

    public CompletionStage<OutgoingResponse> dispatch(IncomingRequest request) throws UserException {
        String token = request.current.ctx.get("token");

        // Check if the request has a valid token in its context.
        if (token == null || !token.equals(_validToken)) {
            System.out.println("Rejecting request with invalid token " + token + "'");
            throw new DispatchException(ReplyStatus.Unauthorized.value(), "Invalid token '" + token + "'");
        }

        System.out.println("Accepting request with token '" + token + "'");

        // Continue the dispatch pipeline.
        return _next.dispatch(request);
    }

    /**
     * Creates an AuthorizationMiddleware.
     *
     * @param next the next Ice.Object in the dispatch pipeline
     * @param validToken the value to check against
     */
    AuthorizationMiddleware(com.zeroc.Ice.Object next, String validToken) {
        _next = next;
        _validToken = validToken;
    }
}
