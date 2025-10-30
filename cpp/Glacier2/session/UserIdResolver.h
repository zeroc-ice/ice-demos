// Copyright (c) ZeroC, Inc.

#ifndef USER_ID_RESOLVER_H
#define USER_ID_RESOLVER_H

#include <memory>
#include <optional>
#include <string>

namespace Server
{
    /// Resolves a user ID from a session token.
    class UserIdResolver
    {
    public:
        /// Gets the user ID associated with the specified session token.
        /// @param token The session token.
        /// @returns The user ID associated with the specified session token, or nullopt if not found.
        virtual std::optional<std::string> getUserId(const std::string& token) const = 0;

        /// Removes the specified session token.
        /// @param token The session token.
        virtual void removeToken(const std::string& token) = 0;
    };

    using UserIdResolverPtr = std::shared_ptr<UserIdResolver>;
}

#endif
