// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package Filesystem;

public class FileI implements File
{
    // FileI constructor

    public FileI(com.zeroc.Ice.Communicator communicator, String name, DirectoryI parent)
    {
        _name = name;
        _parent = parent;

        assert(_parent != null);

        //
        // Create an identity
        //
        _id = new com.zeroc.Ice.Identity();
        _id.name = java.util.UUID.randomUUID().toString();
    }

    // Slice Node::name() operation

    public String name(com.zeroc.Ice.Current current)
    {
        return _name;
    }

    // Slice File::read() operation

    public String[] read(com.zeroc.Ice.Current current)
    {
        return _lines;
    }

    // Slice File::write() operation

    public void write(String[] text, com.zeroc.Ice.Current current)
        throws GenericError
    {
        _lines = text;
    }

    public void activate(com.zeroc.Ice.ObjectAdapter a)
    {
        NodePrx thisNode = NodePrx.uncheckedCast(a.add(this, _id));
        _parent.addChild(thisNode);
    }

    private String _name;
    private DirectoryI _parent;
    private com.zeroc.Ice.Identity _id;
    private String[] _lines;
}
