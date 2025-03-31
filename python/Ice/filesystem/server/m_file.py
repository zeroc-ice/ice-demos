# Copyright (c) ZeroC, Inc.

from Filesystem import File
from m_node import MNode
import Ice

class MFile(File, MNode):
    """
    Provides an in-memory implementation of the Slice interface File.
    """

    def __init__(self, name):
        """
        Initializes a MFile with the given name.

        Parameters
        ----------
        name : str
            The name of the file.
        """
        super().__init__(name)
        self._lines = []

    def read(self, current: Ice.Current) -> [str]:
        return self._lines

    def write(self, text: [str], current: Ice.Current) -> None:
        self.writeDirect(text)

    def writeDirect(self, text: [str]) -> None:
        """
        Writes directly to this file, without going through an Ice operation.

        Parameters
        ----------
        text : [str]
            The text to write.
        """
        self._lines = text
