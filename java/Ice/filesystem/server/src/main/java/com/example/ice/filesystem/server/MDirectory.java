// Copyright (c) ZeroC, Inc.

package com.example.ice.filesystem.server;

import com.zeroc.Ice.Current;

import java.util.ArrayList;
import java.util.List;

/**
 * Provides an in-memory implementation of the Slice interface Directory.
 */
public class MDirectory extends Node implements Directory {
    private ArrayList<NodePrx> _contents = new ArrayList<>();

    /**
     * Constructs an {@code MDirectory}.
     * @param name the name of this directory
     */
    MDirectory(String name)
    {
        super(name);
    }

    @Override
    public List<NodePrx> list(Current current)
    {
        return _contents;
    }

    /**
     * Adds a node to this directory.
     * @param child the node proxy to add
     */
    void addChild(NodePrx child)
    {
        _contents.add(child);
    }
}
