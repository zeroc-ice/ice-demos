// Copyright (c) ZeroC, Inc.

namespace Server;

/// <summary>Resolves a user ID from a session token.</summary>
internal interface IUserIdResolver
{
    /// <summary>Gets the user ID associated with the specified session token.</summary>
    /// <param name="token">The session token. </param>
    /// <returns>The user ID associated with the specified session token, or null if not found.</returns>
    string? GetUserId(string token);

    /// <summary>Removes the specified session token.</summary>
    /// <param name="token">The session token.</param>
    void RemoveToken(string token);
}
