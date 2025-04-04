#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import sys

# Slice module Filesystem in Filesystem.ice maps to C# namespace Filesystem.
from Filesystem import DirectoryPrx, FilePrx

async def list_recursive(directory, depth=0):
    """
    Recursively list the contents of a directory.

    Parameters
    ----------
    directory : DirectoryPrx
        The directory to list.
    depth : int, optional
        The current depth in the directory hierarchy (default is 0).
    """
    depth += 1
    indent = '\t' * depth

    contents = await directory.listAsync()

    for node in contents:
        assert node is not None # The node proxies returned by list() are never null.

        subdir = await DirectoryPrx.checkedCastAsync(node)
        kind = "directory" if subdir is not None else "file"
        nodeName = await node.nameAsync()

        print(f"{indent}{nodeName} {kind}:")

        if subdir is not None:
            await list_recursive(subdir, depth)
        else:
            file = FilePrx.uncheckedCast(node)
            lines = await file.readAsync()
            for line in lines:
                print(f"{indent}\t{line}")

async def main():
    # Create an Ice communicator. We'll use this communicator to create proxies, and manage outgoing connections. We
    # enable asyncio support by passing the current event loop to initialize.
    async with Ice.initialize(sys.argv, eventLoop=asyncio.get_running_loop()) as communicator:

        # Create a proxy for the root directory.
        rootDir = DirectoryPrx(communicator, "RootDir:tcp -h localhost -p 4061")

        # Recursively list the contents of the root directory.
        print("Contents of root directory:")
        await list_recursive(rootDir)

if __name__ == "__main__":
    asyncio.run(main())
