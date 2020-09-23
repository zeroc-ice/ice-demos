// Copyright (c) ZeroC, Inc. All rights reserved.

using Filesystem;
using System;
using ZeroC.Ice;

using var communicator = new Communicator(ref args);

// Create a proxy for the root directory

IDirectoryPrx rootDir = IDirectoryPrx.Parse("ice+tcp://localhost:10000/RootDir", communicator) ??
    throw new ArgumentException("invalid proxy");

// Recursively list the contents of the root directory
Console.WriteLine("Contents of root directory:");
ListRecursive(rootDir, 0);

// Recursively print the contents of directory "dir" in tree fashion.
// For files, show the contents of each file. The "depth"
// parameter is the current nesting level (for indentation).

static void ListRecursive(IDirectoryPrx dir, int depth)
{
    string indent = new string('\t', ++depth);

    foreach (INodePrx? node in dir.List())
    {
        if (node!.CheckedCast(IDirectoryPrx.Factory) is IDirectoryPrx subdir)
        {
            Console.WriteLine($"{indent}{node.Name()} (directory):");
            ListRecursive(subdir, depth);
        }
        else
        {
            Console.WriteLine($"{indent}{node.Name()} (file):");
            IFilePrx file = node.Clone(factory: IFilePrx.Factory);
            string[] text = file.Read();
            for (int j = 0; j < text.Length; ++j)
            {
                Console.WriteLine(indent + "\t" + text[j]);
            }
        }
    }
}
