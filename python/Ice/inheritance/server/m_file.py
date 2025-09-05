# Copyright (c) ZeroC, Inc.

import Ice
from Filesystem import File
from m_node import MNode


class MFile(File, MNode):
    """
    Provides an in-memory implementation of the Slice interface File.
    """

    def __init__(self, name: str):
        """
        Initializes an MFile with the given name.

        Parameters
        ----------
        name : str
            The name of the file.
        """
        super().__init__(name)
        self._lines = []

    def read(self, current: Ice.Current) -> list[str]:
        return self._lines

    def write(self, text: list[str], current: Ice.Current) -> None:
        self.write_direct(text)

    def write_direct(self, text: list[str]) -> None:
        """
        Writes directly to this file, without going through an Ice operation.

        Parameters
        ----------
        text : [str]
            The text to write.
        """
        self._lines = text
