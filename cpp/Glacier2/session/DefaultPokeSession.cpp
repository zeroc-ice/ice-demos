// Copyright (c) ZeroC, Inc.

#include "DefaultPokeSession.h"

using namespace Server;

DefaultPokeSession::DefaultPokeSession(
            Ice::ObjectAdapterPtr adapter,
            Glacier2::SessionControlPrx sessionControl,
            IUserIdResolver userIdResolver)
    : _adapter(std::move(adapter)),
      _sessionControl(std::move(sessionControl)),
      _userIdResolver(std::move(userIdResolver))
{
}

std::optional<CatchThemAll::PokeBoxPrx> DefaultPokeSession::getPokeBox(const Ice::Current&)
{
    // Implement your logic to retrieve the PokeBox proxy associated with this session.
    return std::nullopt;
}