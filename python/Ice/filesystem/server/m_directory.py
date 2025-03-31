# Copyright (c) ZeroC, Inc.

from Filesystem import Directory, NodePrx
from m_node import MNode
import Ice

class MDirectory(Directory, MNode):

    def __init__(self, name):
        """
        Initializes a MFile with the given name.

        Parameters
        ----------
        name : str
            The name of the file.
        """
        super().__init__(name)
        self._contents = []

    def list(self, current: Ice.Current) -> [NodePrx]:
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
