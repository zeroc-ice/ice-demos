// Copyright (c) ZeroC, Inc.

#include "IUserIdResolver.h"
#include <Glacier2/Glacier2.h>
#include <Ice/Ice.h>

namespace Server
{

    /// <summary>SessionManager is an Ice servant that implements Slice interface Glacier2::SessionManager. It creates
    /// sessions and resolves user IDs from session tokens.</summary>
    /// <remarks>This demo implementation is not thread-safe. A real implementation should support concurrent calls.
    /// </remarks>
    class SessionManager : public Glacier2::SessionManager,
                           public IUserIdResolver,
                           public std::enable_shared_from_this<SessionManager>
    {
    public:
        std::optional<Glacier2::SessionPrx> create(
            std::string userId,
            std::optional<Glacier2::SessionControlPrx> sessionControl,
            const Ice::Current& current) final;

        std::optional<std::string> getUserId(const std::string& token) final;

        void removeToken(const std::string& token) final;

        /// Constructs a SessionManager servant.
        /// @param adapter The object adapter that hosts the session servants.
        SessionManager(Ice::ObjectAdapterPtr adapter);

    private:
        Ice::ObjectAdapterPtr _sessionAdapter;
        std::map<std::string, std::string> _tokenToUserId;
    };

}
