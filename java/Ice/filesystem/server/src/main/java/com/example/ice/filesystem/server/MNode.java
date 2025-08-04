// Copyright (c) ZeroC, Inc.

package com.example.ice.filesystem.server;

import com.example.filesystem.Node;

import com.zeroc.Ice.Current;

/**
 * Provides an in-memory implementation of the Slice interface Node.
 */
class MNode implements Node {
    private final String _name;

    /**
     * Constructs an {@code MNode}.
     *
     * @param name the name of this node
     */
    MNode(String name) {
        _name = name;
    }

    @Override
    public String name(Current current) {
        return _name;
    }
}
