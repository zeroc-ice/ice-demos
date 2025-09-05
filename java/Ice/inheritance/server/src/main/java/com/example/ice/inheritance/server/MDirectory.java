// Copyright (c) ZeroC, Inc.

package com.example.ice.inheritance.server;

import com.example.filesystem.Directory;
import com.example.filesystem.NodePrx;

import java.util.ArrayList;

import com.zeroc.Ice.Current;

/**
 * Provides an in-memory implementation of the Slice interface Directory.
 */
class MDirectory extends MNode implements Directory {
    private final ArrayList<NodePrx> _contents = new ArrayList<>();

    /**
     * Constructs an {@code MDirectory}.
     *
     * @param name the name of this directory
     */
    MDirectory(String name) {
        super(name);
    }

    @Override
    public NodePrx[] list(Current current) {
        return _contents.toArray(new NodePrx[0]);
    }

    /**
     * Adds a node to this directory
     *
     * @param child the node proxy to add
     */
    void addChild(NodePrx child) {
        _contents.add(child);
    }
}
