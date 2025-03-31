#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import Ice
import asyncio
import sys

# Slice module Filesystem in Filesystem.ice maps to C# namespace Filesystem.
from Filesystem import DirectoryPrx, FilePrx

async def main():
    # Create an Ice communicator to initialize the Ice runtime. It will be automatically destroyed at the end of the
    # with block. We enable asyncio support by passing the current event loop to initialize.
    with Ice.initialize(sys.argv, eventLoop=asyncio.get_running_loop()) as communicator:

        # Create a proxy for the root directory.
        rootDir = DirectoryPrx(communicator, "RootDir:tcp -h localhost -p 4061")

        async def list_recursive(directory, depth=0):
            depth += 1
            indent = '\t' * depth

            contents = await directory.listAsync()

            for node in contents:
                assert node is not None # The node proxies returned by list() are never null.

                # TODO add checkedCastAsync to Python
                subdir = DirectoryPrx.checkedCast(node)
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

        # Recursively list the contents of the root directory.
        print("Contents of root directory:")
        await list_recursive(rootDir)

if __name__ == "__main__":
    asyncio.run(main())
