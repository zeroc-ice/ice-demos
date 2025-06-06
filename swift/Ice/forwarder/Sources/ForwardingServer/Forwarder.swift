// Copyright (c) ZeroC, Inc.

import Ice

/// Forwarder is an Ice dispatcher that implements dispatch by forwarding all requests it receives to a remote
/// Ice object.
class Forwarder: Ice.Dispatcher {
    let targetTemplate: Ice.ObjectPrx

    init(_ targetTemplate: Ice.ObjectPrx) {
        self.targetTemplate = targetTemplate.ice_twoway()  // ensure the targetTemplate field is two-way.
    }

    // Dispatches an incoming request by forwarding it to the target Ice object.
    func dispatch(_ request: Ice.IncomingRequest) async throws -> Ice.OutgoingResponse {
        // Create a proxy with the desired identity and facet.
        var target = targetTemplate.ice_identity(request.current.id).ice_facet(request.current.facet)

        if request.current.requestId == 0 {
            // The incoming request is one-way, so we reconfigure target to be one-way.
            target = target.ice_oneway()
        }

        // Make the invocation asynchronously. If ice_invoke throws an Ice.LocalException that cannot be marshaled
        // (such as Ice.ConnectionRefusedException), the object adapter that calls dispatch converts it into an
        // Ice.UnknownLocalException.

        let (ok, encapsulation) = try await target.ice_invoke(
            operation: request.current.operation,
            mode: request.current.mode,
            inEncaps: request.inputStream.readEncapsulation().bytes,
            context: request.current.ctx)

        // Create and return the OutgoingResponse.
        return request.current.makeOutgoingResponse(ok: ok, encapsulation: encapsulation)
    }
}
