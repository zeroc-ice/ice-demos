//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice
import PromiseKit

class InterceptorI: Ice.Disp {
    private let _dispatcher: Ice.Disp
    private let _authenticator: AuthenticatorI
    private let _securedOperations: Set<String>

    init(dispatcher: Ice.Disp, authenticator: AuthenticatorI, securedOperations: Set<String>) {
        _dispatcher = dispatcher
        _authenticator = authenticator
        _securedOperations = securedOperations
    }

    func dispatch(request: Ice.Request, current: Ice.Current) throws -> Promise<Ice.OutputStream>? {
        //
        // Check if the operation requires authorization to invoke.
        //
        if _securedOperations.contains(current.operation) {
            //
            // Validate the client's access token before dispatching to the servant.
            // 'validateToken' throws an exception if the token is invalid or expired.
            //
            guard let tokenValue = current.ctx["accessToken"] else {
                //
                // If the client didn't include an access token, throw an exception.
                //
                throw AuthorizationException()
            }
            try _authenticator.validateToken(tokenValue: tokenValue)
        }

        return try _dispatcher.dispatch(request: request, current: current)
    }
}
