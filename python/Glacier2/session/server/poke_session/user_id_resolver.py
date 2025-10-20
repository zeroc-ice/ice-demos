# Copyright (c) ZeroC, Inc.

from typing import Protocol, runtime_checkable


@runtime_checkable
class UserIdResolver(Protocol):
    """
    Resolves a user ID from a session token.
    """

    def getUserId(self, token: str) -> str | None:
        """
        Gets the user ID associated with the specified session token, or None if not found.

        Parameters
        ----------
        token : str
            The session token.

        Returns
        -------
        str | None
            The user ID associated with the specified session token, or None if not found.
        """
        ...

    def removeToken(self, token: str) -> None:
        """
        Removes the specified session token.

        Parameters
        ----------
        token : str
            The session token.
        """
        ...
