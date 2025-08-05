# Copyright (c) ZeroC, Inc.

import Ice
from Filesystem import Directory, NodePrx
from m_node import MNode


class MDirectory(Directory, MNode):
    """
    Provides an in-memory implementation of the Slice interface Directory.
    """

    def __init__(self, name: str):
        """
        Initializes an MDirectory with the given name.

        Parameters
        ----------
        name : str
            The name of the directory.
        """
        super().__init__(name)
        self._contents = []

    def list(self, current: Ice.Current) -> list[NodePrx]:
        return self._contents

    def addChild(self, child: NodePrx) -> None:
        """
        Adds a node to this directory.

        Parameters
        ----------
        child : NodePrx
            The node to add.
        """
        self._contents.append(child)
