//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Manual.simpleFilesystem.Filesystem.*;

class NodeI implements Node
{
    // NodeI constructor
    NodeI(String name, DirectoryI parent)
    {
        _name = name;
        _parent = parent;

        //
        // Create an identity
        //
        _id = new com.zeroc.Ice.Identity();
        _id.name = parent == null ? "RootDir" : java.util.UUID.randomUUID().toString();
    }

    // Slice Node::name() operation
    @Override public String name(com.zeroc.Ice.Current current)
    {
        return _name;
    }

    void activate(com.zeroc.Ice.ObjectAdapter a)
    {
        NodePrx thisNode = NodePrx.uncheckedCast(a.add(this, _id));
        if(_parent != null)
        {
            _parent.addChild(thisNode);
        }
    }

    private String _name;
    private DirectoryI _parent;
    private com.zeroc.Ice.Identity _id;
}
