// Copyright (c) ZeroC, Inc.

package com.example.ice.forwarder.forwardingserver;

import com.zeroc.Ice.IncomingRequest;
import com.zeroc.Ice.ObjectPrx;
import com.zeroc.Ice.OutgoingResponse;
import com.zeroc.Ice.UserException;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CompletionStage;

/**
 * Forwarder is an Ice servant that implements com.zeroc.Ice.Object by forwarding all requests it receives to a remote
 * Ice object.
 */
class Forwarder implements com.zeroc.Ice.Object {
    // A template for the target proxy.
    private final ObjectPrx _targetTemplate;

    // Implements the abstract method dispatch defined on com.zeroc.Ice.Object.
    @Override
    public CompletionStage<OutgoingResponse> dispatch(IncomingRequest request) throws UserException {
        // Create a proxy with the desired identity and facet.
        ObjectPrx target = _targetTemplate.ice_identity(request.current.id).ice_facet(request.current.facet);

        if (request.current.requestId == 0) {
            // The incoming request is one-way, so we reconfigure target to be one-way.
            target = target.ice_oneway();
        }

        // Make the invocation asynchronously.
        CompletableFuture<com.zeroc.Ice.Object.Ice_invokeResult> future = target.ice_invokeAsync(
            request.current.operation,
            request.current.mode,
            request.inputStream.readEncapsulation(null), // the in-parameters encapsulation
            request.current.ctx);

        // Upon success, create an OutgoingResponse from the invokeResult. If the invocation fails with an exception,
        // the object adapter calling this method will convert the exception into an UnknownLocalException or
        // UnknownException.
        return future.thenApply(
            invokeResult -> request.current.createOutgoingResponse(invokeResult.returnValue, invokeResult.outParams));
    }

    // Constructs a forwarder.
    Forwarder(ObjectPrx targetTemplate) {
        _targetTemplate = targetTemplate.ice_twoway();
    }
}
