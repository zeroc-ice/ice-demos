// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Filesystem.*;

public class Client
{
    // Recursively print the contents of directory "dir" in tree fashion.
    // For files, show the contents of each file. The "depth"
    // parameter is the current nesting level (for indentation).

    static void
    listRecursive(DirectoryPrx dir, int depth)
    {
        char[] indentCh = new char[++depth];
        java.util.Arrays.fill(indentCh, '\t');
        String indent = new String(indentCh);

        NodePrx[] contents = dir.list();

        for(int i = 0; i < contents.length; ++i)
        {
            DirectoryPrx subdir = DirectoryPrxHelper.checkedCast(contents[i]);
            FilePrx file = FilePrxHelper.uncheckedCast(contents[i]);
            System.out.println(indent + contents[i].name() + (subdir != null ? " (directory):" : " (file):"));
            if(subdir != null)
            {
                listRecursive(subdir, depth);
            }
            else
            {
                String[] text = file.read();
                for(int j = 0; j < text.length; ++j)
                {
                    System.out.println(indent + "\t" + text[j]);
                }
            }
        }
    }

    public static void
    main(String[] args)
    {
        try(Ice.Communicator ic = Ice.Util.initialize(args))
        {
            //
            // Create a proxy for the root directory
            //
            Ice.ObjectPrx base = ic.stringToProxy("RootDir:default -h localhost -p 10000");

            //
            // Down-cast the proxy to a Directory proxy
            //
            DirectoryPrx rootDir = DirectoryPrxHelper.checkedCast(base);
            if(rootDir == null)
            {
                throw new RuntimeException("Invalid proxy");
            }

            //
            // Recursively list the contents of the root directory
            //
            System.out.println("Contents of root directory:");
            listRecursive(rootDir, 0);
        }
    }
}
