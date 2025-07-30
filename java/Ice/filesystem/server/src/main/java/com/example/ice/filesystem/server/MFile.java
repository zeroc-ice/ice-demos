// Copyright (c) ZeroC, Inc.

package com.example.ice.filesystem.server;

import com.zeroc.Ice.Current;

/**
 * Provides an in-memory implementation of the Slice interface File.
 */
public class MFile extends Node implements File {
    private String[] _lines = {};

    /**
     * Constructs an {@code MFile}.
     * @param name the name of this file
     */
    MFile(String name)
    {
        super(name);
    }

    @Override
    public String[] read(Current current)
    {
        return _lines;
    }

    @Override
    public void write(String[] text, Current current) throws WriteException
    {
        writeDirect(text);
    }

    /**
     * Writes directly to this file, without going through an Ice operation.
     * @param text the text to write
     */
    void writeDirect(String[] text)
    {
        _lines = text;
    }
}
