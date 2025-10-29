// Copyright (c) ZeroC, Inc.

#include "DefaultPokeSession.h"

#include <iostream>

using namespace Server;
using namespace std;

DefaultPokeSession::DefaultPokeSession(
    Ice::ObjectAdapterPtr adapter,
    Glacier2::SessionControlPrx sessionControl,
    IUserIdResolverPtr userIdResolver)
    : _adapter(std::move(adapter)),
      _sessionControl(std::move(sessionControl)),
      _userIdResolver(std::move(userIdResolver))
{
}

void
DefaultPokeSession::destroy(const Ice::Current& current)
{
    std::cout << "Destroying session #" << current.id.name << std::endl;

    // Remove the token from the user ID resolver, since the token is no longer valid.
    _userIdResolver->removeToken(current.id.name);

    // Remove this servant from the object adapter. A new call to this session object will fail with
    // ObjectNotExistException.
    _adapter->remove(current.id);

    // Destroy the session in the Glacier2 router.
    _sessionControl->destroy();
}

optional<CatchThemAll::PokeBoxPrx>
DefaultPokeSession::getPokeBox(const Ice::Current& current)
{
    // The session token is the name component of the session identity; we use it for the identity of the PokeBox
    // object as well.
    return _adapter->createProxy<CatchThemAll::PokeBoxPrx>(Ice::Identity{current.id.name, "PokeBox"});
}
