// Copyright (c) ZeroC, Inc.

namespace Server;

/// <summary>Provides an in-memory implementation of the Slice interface Directory.</summary>
internal class MDirectory : Filesystem.DirectoryDisp_
{
    private readonly MNode _node; // reuse the MNode implementation
    private readonly List<Filesystem.NodePrx> _contents = [];

    /// <inheritdoc/>
    // Implements Node.Name by forwarding to _node.
    public override string Name(Ice.Current current) => _node.Name(current);

    /// <inheritdoc/>
    public override Filesystem.NodePrx[] List(Ice.Current current) => _contents.ToArray();

    /// <summary>Constructs a MDirectory.</summary>
    /// <param name="name">The name of this directory.</param>
    internal MDirectory(string name) => _node = new MNode(name);

    /// <summary>Adds a node to this directory.</summary>
    /// <param name="child">The node proxy to add.</param>
    internal void AddChild(Filesystem.NodePrx child) => _contents.Add(child);
}
