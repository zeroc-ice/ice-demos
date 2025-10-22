# Copyright (c) ZeroC, Inc.

from collections.abc import Awaitable
from typing import override

import CatchThemAll
import Glacier2
import Ice

from .user_id_resolver import UserIdResolver


class DefaultPokeSession(CatchThemAll.PokeSession):
    """
    DefaultPokeSession is an Ice servant that implements Slice interface PokeSession. We create a
    DefaultPokeSession for each PokeSession object.
    """

    def __init__(
        self, adapter: Ice.ObjectAdapter, sessionControl: Glacier2.SessionControlPrx, userIdResolver: UserIdResolver
    ):
        """
        Constructs a DefaultPokeSession servant.

        Parameters
        ----------
        adapter : Ice.ObjectAdapter
            The object adapter that hosts this servant and the PokeBox objects.
        sessionControl : Glacier2.SessionControlPrx
            The session control proxy.
        userIdResolver : UserIdResolver
            The user ID resolver.
        """
        self._adapter = adapter
        self._sessionControl = sessionControl
        self._userIdResolver = userIdResolver

    @override
    def getPokeBox(self, current: Ice.Current) -> CatchThemAll.PokeBoxPrx:
        # The session token is the name component of the session identity; we use it for the identity of the PokeBox
        # object as well.
        proxy = self._adapter.createProxy(Ice.Identity(name=current.id.name, category="PokeBox"))
        return CatchThemAll.PokeBoxPrx.uncheckedCast(proxy)

    @override
    async def destroy(self, current: Ice.Current) -> None:
        print(f"Destroying session #{current.id.name}")

        # Remove the token from the user ID resolver, since the token is no longer valid.
        self._userIdResolver.removeToken(current.id.name)

        # Remove this servant from the object adapter. A new call to this session object will fail with
        # ObjectNotExistException.
        self._adapter.remove(current.id)

        # Destroy the session in the Glacier2 router.
        await self._sessionControl.destroyAsync()
