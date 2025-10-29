// Copyright (c) ZeroC, Inc.

#include "SessionManager.h"
#include "DefaultPokeSession.h"

#include <iostream>

using namespace Server;
using namespace std;

SessionManager::SessionManager(Ice::ObjectAdapterPtr adapter) : _sessionAdapter(std::move(adapter)) {}

std::optional<Glacier2::SessionPrx>
SessionManager::create(
    std::string userId,
    std::optional<Glacier2::SessionControlPrx> sessionControl,
    const Ice::Current&)
{
    // sessionControl is not null because we configured Glacier2.Server.Endpoints in the Glacier2 router
    // configuration file.
    assert(sessionControl);

    // Create a new session servant and add it to the adapter with a UUID identity. The name component of the
    // identity is the session token.
    auto proxy = _sessionAdapter->addWithUUID<Glacier2::SessionPrx>(
        make_shared<DefaultPokeSession>(_sessionAdapter, *sessionControl, shared_from_this()));

    string sessionToken = proxy.ice_getIdentity().name;
    _tokenToUserId[sessionToken] = userId;

    cout << "Created session #" << sessionToken << " for user '" << userId << "'" << endl;
    return proxy;
}

std::optional<std::string>
SessionManager::getUserId(const std::string& token)
{
    const auto& it = _tokenToUserId.find(token);
    return it == _tokenToUserId.end() ? std::nullopt : std::optional<std::string>{it->second};
}

void
SessionManager::removeToken(const std::string& token)
{
    _tokenToUserId.erase(token);
}
