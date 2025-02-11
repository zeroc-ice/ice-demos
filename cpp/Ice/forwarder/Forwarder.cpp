// Copyright (c) ZeroC, Inc.

#include "Forwarder.h"

using namespace std;

ForwardingServer::Forwarder::Forwarder(Ice::ObjectPrx targetTemplate)
    : _targetTemplate{std::move(targetTemplate)}
{
}

void
ForwardingServer::Forwarder::dispatch(Ice::IncomingRequest& request, function<void(Ice::OutgoingResponse)> sendResponse)
{
    // Make a copy of the current object carried by the request.
    const Ice::Current current{request.current()};

    // Create a proxy with the the desired identity and facet.
    Ice::ObjectPrx target = _targetTemplate.ice_identity(current.id).ice_facet(current.facet);

    if (current.requestId == 0)
    {
        // The incoming request is one-way, so we reconfigure target to be one-way.
        target = target.ice_oneway();
    }

    // Read the encapsulation containing the in-parameters.
    const std::byte* inEncapsulationStart = nullptr;
    int32_t inEncapsulationSize = 0;
    request.inputStream().readEncapsulation(inEncapsulationStart, inEncapsulationSize);

    // Make the invocation asynchronously. This call reports most exceptions through its exception callback.
    target.ice_invokeAsync(
        current.operation,
        current.mode,
        make_pair(inEncapsulationStart, inEncapsulationStart + inEncapsulationSize),
        [sendResponse, current](bool ok, pair<const std::byte*, const std::byte*> outEncapsulation)
        {
            // The response callback, executed by a thread from the Ice client thread pool when the invocation
            // completes successfully (ok is true) or with a user exception (ok is false).
            // We create an OutgoingResponse object and send it back to the client with sendResponse.
            sendResponse(Ice::makeOutgoingResponse(ok, outEncapsulation, current));
        },
        [sendResponse, current](std::exception_ptr exceptionPtr)
        {
            // The exception callback.
            // We create an OutgoingResponse object with the exception and send it back to the client with
            // sendResponse. If the exception is an Ice local exception that cannot be marshaled, such as
            // Ice::ConnectionRefusedException, makeOutgoingResponse marshals an Ice::UnknownLocalException.
            sendResponse(Ice::makeOutgoingResponse(exceptionPtr, current));
        },
        nullptr, // no sent callback
        current.ctx);
}
