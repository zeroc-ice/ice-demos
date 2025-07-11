# Copyright (c) ZeroC, Inc.

import Ice
from Filesystem import Node


class MNode(Node):
    """
    Provides an in-memory implementation of the Slice interface Node.
    """

    def __init__(self, name: str):
        """
        Initializes a MNode with the given name.

        Parameters
        ----------
        name : str
            The name of the file.
        """
        self._name = name

    def name(self, _: Ice.Current) -> str:
        return self._name
