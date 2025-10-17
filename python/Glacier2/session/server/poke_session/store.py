# Copyright (c) ZeroC, Inc.

from typing import Protocol, Sequence, runtime_checkable


@runtime_checkable
class PokeStore(Protocol):
    """
    Represents a Pokémon storage system.
    """

    def retrieveCollection(self, user_id: str) -> Sequence[str] | None:
        """
        Retrieves the Pokémon collection for a specific user.

        Parameters
        ----------
        user_id : str
            The user ID.

        Returns
        -------
        Sequence[str] | None
            The saved Pokémon collection, or None if no collection was saved for the user.
        """
        ...

    def saveCollection(self, user_id: str, pokemon: Sequence[str]) -> None:
        """
        Saves the Pokémon collection for a specific user.

        Parameters
        ----------
        user_id : str
            The user ID.
        pokemon : Sequence[str]
            The Pokémon collection to save.
        """
        ...


class InMemoryPokeStore:
    def __init__(self):
        self._store = {}

    def retrieveCollection(self, user_id):
        return self._store.get(user_id, None)

    def saveCollection(self, user_id, pokemon):
        if len(pokemon) == 0:
            if user_id in self._store:
                del self._store[user_id]
        else:
            self._store[user_id] = tuple(pokemon)
