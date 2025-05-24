// Copyright (c) ZeroC, Inc.

package com.example.ice.forwarder.forwarderserver;

import com.zeroc.Ice.EncodingVersion;
import com.zeroc.Ice.IncomingRequest;
import com.zeroc.Ice.LocalException;
import com.zeroc.Ice.Object;
import com.zeroc.Ice.ObjectPrx;
import com.zeroc.Ice.OutgoingResponse;
import com.zeroc.Ice.UserException;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CompletionStage;

// Forwarder is an Ice servant that implements Ice.Object by forwarding all requests it receives to a remote Ice object.
class Forwarder implements Object{
    // A template for the target proxy.
    private final ObjectPrx _targetTemplate;

    // Implements abstract method dispatch defined on Ice.Object.
    public CompletionStage<OutgoingResponse> dispatch(IncomingRequest request) throws UserException, LocalException{
        // Create a proxy with the desired identity and facet.
        ObjectPrx target = _targetTemplate.ice_identity(request.current.id).ice_facet(request.current.facet);

        if (request.current.requestId == 0)
        {
            // The incoming request is one-way, so we reconfigure target to be one-way.
            target = target.ice_oneway();
        }

        // Make the invocation synchronously. If ice_invoke throws an Ice.LocalException that cannot be
        // marshaled (such as Ice.ConnectionRefusedException), the object adapter that calls dispatch converts it
        // into an Ice.UnknownLocalException.
        CompletableFuture<Object.Ice_invokeResult> result = target.ice_invokeAsync(
            request.current.operation,
            request.current.mode,
            request.inputStream.readEncapsulation(target.ice_getEncodingVersion()), // the in-parameters encapsulation
            request.current.ctx);

        // Create and return the OutgoingResponse.
        return result.thenApply(response -> request.current.createOutgoingResponse(
            response,
            (ostr, result.get()) -> { ostr.writeProxy(result.get()); }, 
            null));
    }

    Forwarder(ObjectPrx targetTemplate) {_targetTemplate = targetTemplate.ice_twoway();}
}
