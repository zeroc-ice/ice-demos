// Copyright (c) ZeroC, Inc.

import Ice

/// The AuthorizationMiddleware intercepts incoming requests and checks the token entry in their contexts.
final class AuthorizationMiddleware: Ice.Dispatcher {
    private let next: Dispatcher
    private let validToken: String

    /// Initializes an AuthorizationMiddleware.
    /// - Parameters:
    ///   - next: The next dispatcher in the dispatch pipeline.
    ///   - validToken: The value to check against.
    init(next: Dispatcher, validToken: String) {
        self.next = next
        self.validToken = validToken
    }

    /// Dispatches the incoming request, checking for a valid token.
    func dispatch(_ request: IncomingRequest) async throws -> OutgoingResponse {
        let token = request.current.ctx["token"] ?? ""

        guard token == validToken else {
            print("Rejecting request with invalid token '\(token)'")
            throw DispatchException(
                replyStatus: ReplyStatus.unauthorized.rawValue, message: "Invalid token '\(token)'")
        }

        print("Accepting request with token '\(token)'")

        // Continue the dispatch pipeline.
        return try await next.dispatch(request)
    }
}
