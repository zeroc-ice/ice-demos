// Copyright (c) ZeroC, Inc.

package com.example.ice.inheritance.client;

import com.example.inheritance.DirectoryPrx;
import com.example.inheritance.FilePrx;
import com.example.inheritance.NodePrx;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Util;

class Client {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = Util.initialize(args)) {

            // Create a proxy for the root directory.
            DirectoryPrx rootDir = DirectoryPrx.createProxy(communicator, "RootDir:tcp -h localhost -p 4061");

            // Recursively list the contents of the root directory.
            System.out.println("Contents of root directory:");
            listRecursive(rootDir, 0);
        }
    }

    /**
     * Recursively prints the contents of a directory in tree fashion.
     * For files, show the contents of each file.
     *
     * @param dir the directory to list
     * @param depth the current nesting level (for indentation)
     */
    private static void listRecursive(DirectoryPrx dir, int depth) {
        String indent = "\t".repeat(++depth);

        NodePrx[] contents = dir.list();

        for (NodePrx node : contents) {
            // The node proxies returned by list() are never null.
            java.util.Objects.requireNonNull(node);

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
