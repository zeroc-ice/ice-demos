// Copyright (c) ZeroC, Inc.

namespace Server;

/// <summary>Provides an in-memory implementation of the Slice interface File.</summary>
internal class MFile : Filesystem.FileDisp_
{
    private readonly string _name;
    private string[] _lines = [];

    /// <inheritdoc/>
    public override string Name(Ice.Current current) => _name;

    /// <inheritdoc/>
    public override string[] Read(Ice.Current current) => _lines;

    /// <inheritdoc/>
    public override void Write(string[] text, Ice.Current current) => WriteDirect(text);

    /// <summary>Constructs a MFile.</summary>
    /// <param name="name">The name of this file.</param>
    internal MFile(string name) => _name = name;

    /// <summary>Writes directly to this file, without going through an Ice operation.</summary>
    /// <param name="text">The text to write.</param>
    internal void WriteDirect(string[] text) => _lines = text;
}
