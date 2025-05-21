// Copyright (c) ZeroC, Inc.

package com.example.ice.middleware.server;

import com.zeroc.Ice.DispatchException;
import com.zeroc.Ice.IncomingRequest;
import com.zeroc.Ice.Object;
import com.zeroc.Ice.OutgoingResponse;
import com.zeroc.Ice.ReplyStatus;
import com.zeroc.Ice.UserException;
import java.util.concurrent.CompletionStage;

/**
 * The AuthorizationMiddleware intercepts incoming requests and checks the token entry in their contexts.
 */
public class AuthorizationMiddleware implements Object {
    private final Object _next;
    private final String _validToken;

    public CompletionStage<OutgoingResponse> dispatch(IncomingRequest request) {
        String token = request.current.ctx.get("token");

        // Check if the request has a valid token in its context.
        if (token == null || !token.equals(_validToken)) {
            System.out.println("Rejecting request with invalid token " + token);
            throw new DispatchException(9, "Invalid token " + token);
        }

        System.out.println("Accepting request with token " + token);
        
        // Continue the dispatch pipeline.
        CompletionStage<OutgoingResponse> response = null;
        try 
        {
            response = _next.dispatch(request);
        }
        catch (UserException exception)
        {
            System.out.println("Caught unexpected UserException " + exception.getMessage());
        }
        return response;
    }

    // Constructs an AuthorizationMiddleware.
    // The next Ice.Object in the dispatch pipeline.
    // The value to check against.
    AuthorizationMiddleware(Object next, String validToken) {
        _next = next;
        _validToken = validToken;
    }
}
