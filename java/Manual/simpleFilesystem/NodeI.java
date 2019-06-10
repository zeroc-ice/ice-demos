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
    }

    // Slice Node::name() operation
    @Override public String name(com.zeroc.Ice.Current current)
    {
        return _name;
    }

    void activate(com.zeroc.Ice.ObjectAdapter a)
    {
        //
        // Create an identity
        //
        com.zeroc.Ice.Identity id = new com.zeroc.Ice.Identity();
        id.name = _parent == null ? "RootDir" : java.util.UUID.randomUUID().toString();

        NodePrx thisNode = NodePrx.uncheckedCast(a.add(this, id));
        if(_parent != null)
        {
            _parent.addChild(thisNode);
        }
    }

    private String _name;
    private DirectoryI _parent;
}
