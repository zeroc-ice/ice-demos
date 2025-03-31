# Copyright (c) ZeroC, Inc.

from Filesystem import Directory, NodePrx
import Ice

class MDirectory(Directory):

    def __init__(self, name):
        """
        Initializes a MDirectory with the given name.

        Parameters
        ----------
        name : str
            The name of the file.
        """
        self._name = name
        self._contents = []


    def name(self, current: Ice.Current) -> str:
        return self._name

    def list(self, current: Ice.Current) -> [NodePrx]:
        return self._contents

    def addChild(self, child: NodePrx) -> None:
        """"
        Adds a node to this directory.

        Parameters
        ----------
        child : NodePrx
            The node to add.
        """
        self._contents.append(child)
