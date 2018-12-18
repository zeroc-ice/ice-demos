// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Filesystem;
using System;

public class Program
{
    // Recursively print the contents of directory "dir" in tree fashion.
    // For files, show the contents of each file. The "depth"
    // parameter is the current nesting level (for indentation).

    static void listRecursive(DirectoryPrx dir, int depth)
    {
        var indent = new string('\t', ++depth);

        NodePrx[] contents = dir.list();

        foreach(var node in contents)
        {
            var subdir = DirectoryPrxHelper.checkedCast(node);
            var file = FilePrxHelper.uncheckedCast(node);
            Console.WriteLine(indent + node.name() + (subdir != null ? " (directory):" : " (file):"));
            if(subdir != null)
            {
                listRecursive(subdir, depth);
            }
            else
            {
                var text = file.read();
                for(int j = 0; j < text.Length; ++j)
                {
                    Console.WriteLine(indent + "\t" + text[j]);
                }
            }
        }
    }

    public static int Main(string[] args)
    {
        try
        {
            using(Ice.Communicator ic = Ice.Util.initialize(ref args))
            {
                //
                // Create a proxy for the root directory
                //
                var obj = ic.stringToProxy("RootDir:default -h localhost -p 10000");

                //
                // Down-cast the proxy to a Directory proxy
                //
                var rootDir = DirectoryPrxHelper.checkedCast(obj);
                if(rootDir == null)
                {
                    throw new ApplicationException("Invalid proxy");
                }

                //
                // Recursively list the contents of the root directory
                //
                Console.WriteLine("Contents of root directory:");
                listRecursive(rootDir, 0);
            }
        }
        catch(Exception e)
        {
            Console.Error.WriteLine(e);
            return 1;
        }
        return 0;
    }
}
