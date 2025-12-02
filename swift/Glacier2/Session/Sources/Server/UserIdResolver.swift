// Copyright (c) ZeroC, Inc.

/// Protocol for resolving user IDs from session tokens.
protocol UserIdResolver: Actor {
    /// Gets the user ID associated with the specified session token.
    /// - Parameter token: The session token.
    /// - Returns: The user ID associated with the token, or nil if not found.
    func getUserId(_ token: String) -> String?

    /// Removes the specified session token.
    /// - Parameter token: The session token to remove.
    func removeToken(_ token: String)
}
