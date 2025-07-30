// Copyright (c) ZeroC, Inc.

package com.example.ice.filesystem.client;

import com.example.filesystem.DirectoryPrx;
import com.example.filesystem.WriteException;

import com.zeroc.Ice.ClassSliceLoader;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.Util;

class Client {
    public static void main(String[] args) {
        // Configures the communicator to use a Slice loader that loads exception WriteException. This is necessary in
        // Java applications that unmarshal classes or exceptions, when the classes/exceptions or their enclosing
        // module(s) are remapped using java:identifier, like in this demo.
        var initData = new InitializationData();
        initData.sliceLoader = new ClassSliceLoader(WriteException.class);

        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = Util.initialize(args, initData)) {

            // Create a proxy for the root directory.
            DirectoryPrx rootDir = DirectoryPrx.createProxy(communicator, "RootDir:tcp -h localhost -p 4061");

            // Recursively list the contents of the root directory.
            System.out.println("Contents of root directory:");
            listRecursive(rootDir, 0);
        }
    }

    /**
     * Recursively prints the contents of a directory in tree fashion. For files, show the contents of each file.
     * @param dir the directory to list
     * @param depth the current nesting level (for indentation)
     */
    private static void listRecursive(DirectoryPrx dir, int depth)
    {

    }
}
