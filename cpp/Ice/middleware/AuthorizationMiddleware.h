// Copyright (c) ZeroC, Inc.

#ifndef AUTHORIZATION_MIDDLEWARE_H
#define AUTHORIZATION_MIDDLEWARE_H

#include <Ice/Ice.h>

namespace Server
{
    /// The AuthorizationMiddleware intercepts incoming requests and checks the token entry in their contexts.
    class AuthorizationMiddleware final : public Ice::Object
    {
    public:
        /// Constructs an AuthorizationMiddleware.
        /// @param next The next Ice::Object in the dispatch pipeline.
        /// @param validToken The value to check against.
        AuthorizationMiddleware(Ice::ObjectPtr next, std::string validToken);

        // Implements the pure virtual function dispatch declared on Ice::Object.
        void dispatch(Ice::IncomingRequest& request, std::function<void(Ice::OutgoingResponse)> sendResponse) final;

    private:
        const Ice::ObjectPtr _next;
        const std::string _validToken;
    };
}

#endif
