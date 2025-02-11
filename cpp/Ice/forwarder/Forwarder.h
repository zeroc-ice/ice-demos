// Copyright (c) ZeroC, Inc.

#ifndef FORWARDER_H
#define FORWARDER_H

#include <Ice/Ice.h>

namespace ForwardingServer
{
    /// Forwarder is an Ice servant that implements Ice::Object by forwarding all requests it receives to a remote
    /// Ice object.
    class Forwarder final : public Ice::Object
    {
    public:
        /// Constructs a Forwarder servant.
        /// @param targetTemplate A template for the target proxy.
        explicit Forwarder(Ice::ObjectPrx targetTemplate);

        // Implements the pure virtual function dispatch declared on Ice::Object.
        void dispatch(Ice::IncomingRequest& request, std::function<void(Ice::OutgoingResponse)> sendResponse) final;

    private:
        Ice::ObjectPrx _targetTemplate;
    };
}

#endif
