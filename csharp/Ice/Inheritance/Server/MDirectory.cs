// Copyright (c) ZeroC, Inc.

namespace Server;

/// <summary>Provides an in-memory implementation of the Slice interface Directory.</summary>
internal class MDirectory : Inheritance.DirectoryDisp_
{
    private readonly MNode _node; // reuse the MNode implementation
    private readonly List<Inheritance.NodePrx> _contents = [];

    /// <inheritdoc/>
    // Implements Node.Name by forwarding to _node.
    public override string Name(Ice.Current current) => _node.Name(current);

    /// <inheritdoc/>
    public override Inheritance.NodePrx[] List(Ice.Current current) => _contents.ToArray();

    /// <summary>Constructs an MDirectory.</summary>
    /// <param name="name">The name of this directory.</param>
    internal MDirectory(string name) => _node = new MNode(name);

    /// <summary>Adds a node to this directory.</summary>
    /// <param name="child">The node proxy to add.</param>
    internal void AddChild(Inheritance.NodePrx child) => _contents.Add(child);
}
