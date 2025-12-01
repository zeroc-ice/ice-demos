// Copyright (c) ZeroC, Inc.

import Foundation
import Glacier2
import Ice

/// Protocol for resolving user IDs from session tokens.
protocol UserIdResolver {
    /// Gets the user ID associated with the specified session token.
    /// - Parameter token: The session token.
    /// - Returns: The user ID associated with the token, or nil if not found.
    func getUserId(_ token: String) -> String?

    /// Removes the specified session token.
    /// - Parameter token: The session token to remove.
    func removeToken(_ token: String)
}
