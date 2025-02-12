// Copyright (c) ZeroC, Inc.

namespace Server;

/// <summary>Provides an in-memory implementation of the Slice interface Directory.</summary>
internal class MDirectory : Filesystem.DirectoryDisp_
{
    private readonly string _name;
    private readonly List<Filesystem.NodePrx> _contents = new();

    /// <inheritdoc/>
    public override string Name(Ice.Current current) => _name;

    /// <inheritdoc/>
    public override Filesystem.NodePrx[] List(Ice.Current current) => _contents.ToArray();

    /// <summary>Constructs a MDirectory.</summary>
    /// <param name="name">The name of this directory.</param>
    internal MDirectory(string name) => _name = name;

    /// <summary>Adds a node to this directory.</summary>
    /// <param name="child">The node proxy to add</param>
    internal void AddChild(Filesystem.NodePrx child) => _contents.Add(child);
}
