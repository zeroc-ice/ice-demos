// Copyright (c) ZeroC, Inc.

#include "AuthorizationMiddleware.h"

#include <iostream>

using namespace std;

Server::AuthorizationMiddleware::AuthorizationMiddleware(Ice::ObjectPtr next, string validToken)
    : _next{std::move(next)},
      _validToken{std::move(validToken)}
{
}

void
Server::AuthorizationMiddleware::dispatch(
    Ice::IncomingRequest& request,
    function<void(Ice::OutgoingResponse)> sendResponse)
{
    // Check if the request has a valid token in its context.
    auto p = request.current().ctx.find("token");
    string token = p != request.current().ctx.end() ? p->second : "";
    if (token != _validToken)
    {
        cout << "Rejecting request with token '" << token << "'" << endl;

        // ObjectNotExistException is one of the 6 Ice local exceptions that Ice transmits "over the wire".
        // It usually means "no servant was found for this identity". Here, we reuse it to indicate an authorization
        // failure without leaking information to the client.
        throw Ice::ObjectNotExistException{__FILE__, __LINE__};
    }

    cout << "Accepting request with token '" << token << "'" << endl;

    // Continue the dispatch pipeline.
    _next->dispatch(request, std::move(sendResponse));
}
