# Copyright (c) ZeroC, Inc.

from Filesystem import Node
import Ice

class MNode(Node):
    """
    Provides an in-memory implementation of the Slice interface File.
    """

    def __init__(self, name):
        """
        Initializes a MNode with the given name.

        Parameters
        ----------
        name : str
            The name of the file.
        """
        self._name = name


    def name(self, current: Ice.Current) -> str:
        return self._name
