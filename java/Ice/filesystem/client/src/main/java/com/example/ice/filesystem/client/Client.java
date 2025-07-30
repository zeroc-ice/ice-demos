// Copyright (c) ZeroC, Inc.

package com.example.ice.filesystem.client;

import com.example.filesystem.DirectoryPrx;
import com.example.filesystem.FilePrx;
import com.example.filesystem.NodePrx;
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
     *
     * @param dir the directory to list
     * @param depth the current nesting level (for indentation)
     */
    private static void listRecursive(DirectoryPrx dir, int depth) {
        String indent = "\t".repeat(++depth);

        // The node proxies returned by list() are never null.
        NodePrx[] contents = dir.list();

        for (NodePrx node : contents) {
            // Check if this node is a directory by asking the remote object.
            DirectoryPrx subdir = DirectoryPrx.checkedCast(node);

            // We assume it's a file if it's not a directory.
            String kind = (subdir != null ? "(directory)" : "(file)");
            String name = node.name();

            System.out.println(indent + name + " " + kind);

            if (subdir != null) {
                listRecursive(subdir, depth);
            } else {
                FilePrx file = FilePrx.uncheckedCast(node);
                String[] lines = file.read();
                for (String line : lines) {
                    System.out.println(indent + '\t' + line);
                }
            }
        }
    }
}
