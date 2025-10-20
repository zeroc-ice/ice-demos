# Copyright (c) ZeroC, Inc.

from typing import Sequence, override

import CatchThemAll
import Ice

from .user_id_resolver import UserIdResolver
from .store import PokeStore


class SharedPokeBox(CatchThemAll.PokeBox):
    """
    SharedPokeBox is an Ice servant that implements Slice interface PokeBox. The same shared servant
    implements all PokeBox objects; this is doable because all the state is stored in the IPokeStore.
    """

    def __init__(self, pokeStore: PokeStore, userIdResolver: UserIdResolver):
        """
        Constructs a SharedPokeBox servant.

        Parameters
        ----------
        pokeStore : PokeStore
            The Poke store.
        userIdResolver : UserIdResolver
            The user ID resolver.
        """
        self._pokeStore = pokeStore
        self._userIdResolver = userIdResolver

    @override
    def getInventory(self, current: Ice.Current) -> Sequence[str]:
        """
        Retrieve the Pokémon collection for the user associated with the current session.
        """
        collection = self._pokeStore.retrieveCollection(self.getUserId(current))
        print(f"User '{self.getUserId(current)}' retrieved their collection: {collection}")
        return collection if collection is not None else []

    @override
    def caught(self, pokemon: list[str], current: Ice.Current) -> None:
        """
        Add new Pokémon to the Pokémon collection for the user associated with the current session.
        """
        userId = self.getUserId(current)

        # Retrieve the existing collection for the user and add the new Pokémon.
        savedPokemon = list(self._pokeStore.retrieveCollection(userId) or [])
        savedPokemon.extend(pokemon)

        # Sort the collection to make it easier to see the Pokémon that have been caught.
        savedPokemon.sort()
        self._pokeStore.saveCollection(userId, savedPokemon)

    @override
    def releaseAll(self, current: Ice.Current) -> None:
        self._pokeStore.saveCollection(self.getUserId(current), [])

    @override
    def getUserId(self, current: Ice.Current) -> str:
        userId = self._userIdResolver.getUserId(current.id.name)
        if userId is None:
            raise Ice.DispatchException(Ice.ReplyStatus.Unauthorized.value, "Invalid session token")
        return userId
