// Copyright (c) ZeroC, Inc.

namespace Server;

/// <summary>Provides an in-memory implementation of the Slice interface Node.</summary>
internal class MNode : Filesystem.NodeDisp_
{
    private readonly string _name;

    /// <inheritdoc/>
    public override string Name(Ice.Current current) => _name;

    /// <summary>Constructs an MNode.</summary>
    /// <param name="name">The name of this node.</param>
    internal MNode(string name) => _name = name;
}
