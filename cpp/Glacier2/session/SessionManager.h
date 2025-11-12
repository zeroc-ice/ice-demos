// Copyright (c) ZeroC, Inc.

#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "UserIdResolver.h"
#include <Glacier2/Glacier2.h>
#include <Ice/Ice.h>

namespace Server
{
    /// SessionManager is an Ice servant that implements Slice interface Glacier2::SessionManager. It creates sessions
    /// and resolves user IDs from session tokens.
    /// @remark This demo implementation is not thread-safe. A real implementation should support concurrent calls.
    class SessionManager : public Glacier2::SessionManager,
                           public UserIdResolver,
                           public std::enable_shared_from_this<SessionManager>
    {
    public:
        /// Constructs a SessionManager servant.
        /// @param adapter The object adapter that hosts the session servants.
        SessionManager(Ice::ObjectAdapterPtr adapter);

        std::optional<Glacier2::SessionPrx> create(
            std::string userId,
            std::optional<Glacier2::SessionControlPrx> sessionControl,
            const Ice::Current& current) final;

        [[nodiscard]] std::optional<std::string> getUserId(const std::string& token) const final;

        void removeToken(const std::string& token) final;

    private:
        Ice::ObjectAdapterPtr _sessionAdapter;
        std::map<std::string, std::string> _tokenToUserId;
    };
}

#endif
