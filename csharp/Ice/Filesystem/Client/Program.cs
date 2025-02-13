// Copyright (c) ZeroC, Inc.

// Slice module Filesystem in Filesystem.ice maps to C# namespace Filesystem.
using Filesystem;
using System.Diagnostics;

// Create an Ice communicator to initialize the Ice runtime.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create a proxy for the root directory.
DirectoryPrx rootDir = DirectoryPrxHelper.createProxy(communicator, "RootDir:tcp -h localhost -p 4061");

// Recursively list the contents of the root directory.
Console.WriteLine("Contents of root directory:");
ListRecursive(rootDir);

/// <summary>Recursively print the contents of a directory in tree fashion. For files, show the contents of each file.
/// </summary>
/// <param name="dir">The directory to list./<param>
/// <param name="depth">The current nesting level (for indentation).</param>
void ListRecursive(DirectoryPrx dir, int depth = 0)
{
    var indent = new string('\t', ++depth);

    NodePrx?[] contents = dir.List();

    foreach (NodePrx? node in contents)
    {
        Debug.Assert(node is not null); // The node proxies returned by list() are never null.

        DirectoryPrx? subdir = DirectoryPrxHelper.checkedCast(node);
        string kind = subdir is not null ? "directory" : "file";

        Console.WriteLine($"{indent}{node.Name()} {kind}:");

        if (subdir is not null)
        {
            ListRecursive(subdir, depth);
        }
        else
        {
            FilePrx file = FilePrxHelper.uncheckedCast(node);
            string[] lines = file.Read();
            foreach (string line in lines)
            {
                Console.WriteLine($"{indent}\t{line}");
            }
        }
    }
}
