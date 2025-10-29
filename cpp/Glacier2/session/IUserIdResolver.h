// Copyright (c) ZeroC, Inc.

#ifndef IUSER_ID_RESOLVER_H
#define IUSER_ID_RESOLVER_H

#include <memory>
#include <optional>
#include <string>

namespace Server
{
    /// Resolves a user ID from a session token.
    class IUserIdResolver
    {
    public:
        /// Gets the user ID associated with the specified session token.
        /// @param token The session token.
        /// @returns The user ID associated with the specified session token, or null if not found.
        virtual std::optional<std::string> getUserId(const std::string& token) = 0;

        /// Removes the specified session token.
        /// @param token The session token.
        virtual void removeToken(const std::string& token) = 0;
    };

    using IUserIdResolverPtr = std::shared_ptr<IUserIdResolver>;
}
#endif
