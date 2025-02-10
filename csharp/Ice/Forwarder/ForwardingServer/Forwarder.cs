// Copyright (c) ZeroC, Inc.

using Ice;
using System.Diagnostics;

namespace ForwardingServer;

/// <summary>Forwarder is an Ice servant that implements Ice.Object by forwarding all requests it receives to a remote
/// Ice object.</summary>
internal class Forwarder : Ice.Object
{
    // A template for the target proxy.
    private readonly Ice.ObjectPrx _target;

    // Implements abstract method dispatchAsync defined on Ice.Object.
    public async ValueTask<OutgoingResponse> dispatchAsync(IncomingRequest request)
    {
        // Change the identity and facet of _target to match in incoming request.
        Ice.ObjectPrx target = _target.ice_identity(request.current.id).ice_facet(request.current.facet);

        if (request.current.requestId == 0)
        {
            // The incoming request is one-way, so we reconfigure target to be one-way.
            target = target.ice_oneway();
        }

        // Make the invocation asynchronously. If `ice_invokeAsync` throws an Ice.LocalException that cannot be
        // marshaled (such as Ice.ConnectionRefusedException), the object adapter that calls `dispatchAsync` converts it
        // into an Ice.UnknownLocalException.
        (bool ok, byte[] encapsulation) = await target.ice_invokeAsync(
            request.current.operation,
            request.current.mode,
            request.inputStream.readEncapsulation(out EncodingVersion _), // the in-parameters encapsulation
            request.current.ctx);

        // Create and return the OutgoingResponse.
        return request.current.createOutgoingResponse(ok, encapsulation);
    }

    /// <summary>Constructs a forwarder.</summary>
    /// <param name="target">A template of the target proxy. It must be two-way.</param>
    internal Forwarder(Ice.ObjectPrx target)
    {
        _target = target;
        Debug.Assert(_target.ice_isTwoway());
    }
}
