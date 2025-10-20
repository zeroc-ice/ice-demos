# Copyright (c) ZeroC, Inc.

from typing import override

import Glacier2
import Ice

from .session import DefaultPokeSession
from .user_id_resolver import UserIdResolver


class SessionManager(Glacier2.SessionManager, UserIdResolver):
    """
    SessionManager is an Ice servant that implements Slice interface Glacier2::SessionManager. It creates
    sessions and resolves user IDs from session tokens.
    """

    def __init__(self, adapter: Ice.ObjectAdapter):
        """
        Constructs a SessionManager servant.

        Parameters
        ----------
        adapter : Ice.ObjectAdapter
            The object adapter that hosts the session servants.
        """
        self._sessionAdapter = adapter
        self._tokenToUserId = {}

    @override
    def create(
        self, userId: str, sessionControl: Glacier2.SessionControlPrx | None, current: Ice.Current
    ) -> Glacier2.SessionPrx:
        # sessionControl is not None because we configured Glacier2.Server.Endpoints in the Glacier2 router
        # configuration file.
        assert sessionControl is not None

        # Create a new session servant and add it to the adapter with a UUID identity. The name component of the
        # identity is the session token.
        proxy = self._sessionAdapter.addWithUUID(DefaultPokeSession(self._sessionAdapter, sessionControl, self))

        sessionToken = proxy.ice_getIdentity().name
        self._tokenToUserId[sessionToken] = userId

        print(f"Created session #{sessionToken} for user '{userId}'")
        return Glacier2.SessionPrx.uncheckedCast(proxy)

    @override
    def getUserId(self, token: str):
        return self._tokenToUserId.get(token, None)

    @override
    def removeToken(self, token: str):
        if token in self._tokenToUserId:
            del self._tokenToUserId[token]
