//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Manual.simpleFilesystem.Filesystem.*;

final class DirectoryI extends NodeI implements Directory
{
    // DirectoryI constructor
    DirectoryI(String name, DirectoryI parent)
    {
        super(name, parent);
    }

    // Slice Directory::list() operation
    @Override public NodePrx[] list(com.zeroc.Ice.Current current)
    {
        NodePrx[] result = new NodePrx[_contents.size()];
        _contents.toArray(result);
        return result;
    }

    // addChild is called by the child in order to add
    // itself to the _contents member of the parent
    void addChild(NodePrx child)
    {
        _contents.add(child);
    }

    private java.util.List<NodePrx> _contents = new java.util.ArrayList<>();
}
