// Copyright (c) ZeroC, Inc.

package com.example.glacier2.session.server;

/** Resolves a user ID from a session token. */
interface UserIdResolver {
    /**
     * Gets the user ID associated with the specified session token.
     * @param token the session token
     * @return the user ID associated with the specified session token, or null if not found
     */
    String getUserId(String token);

    /**
     * Removes the specified session token.
     * @param token the session token
     */
    void removeToken(String token);
}
