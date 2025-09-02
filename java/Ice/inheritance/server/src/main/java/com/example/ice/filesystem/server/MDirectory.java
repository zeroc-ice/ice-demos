// Copyright (c) ZeroC, Inc.

package com.example.ice.inheritance.server;

import com.example.inheritance.Directory;
import com.example.inheritance.NodePrx;

import com.zeroc.Ice.Current;

import java.util.ArrayList;

/**
 * Provides an in-memory implementation of the Slice interface Directory.
 */
class MDirectory extends MNode implements Directory {
    private ArrayList<NodePrx> _contents = new ArrayList<>();

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
