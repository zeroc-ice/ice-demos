// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package Filesystem;

public class FileI extends _FileDisp
{
    // FileI constructor

    public
    FileI(Ice.Communicator communicator, String name, DirectoryI parent)
    {
        _name = name;
        _parent = parent;

        assert(_parent != null);

        //
        // Create an identity
        //
        _id = new Ice.Identity();
        _id.name = java.util.UUID.randomUUID().toString();
    }

    // Slice Node::name() operation

    public String
    name(Ice.Current current)
    {
        return _name;
    }

    // Slice File::read() operation

    public String[]
    read(Ice.Current current)
    {
        return _lines;
    }

    // Slice File::write() operation

    public void
    write(String[] text, Ice.Current current)
        throws GenericError
    {
        _lines = text;
    }

    public void
    activate(Ice.ObjectAdapter a)
    {
        NodePrx thisNode = NodePrxHelper.uncheckedCast(a.add(this, _id));
        _parent.addChild(thisNode);
    }

    private String _name;
    private DirectoryI _parent;
    private Ice.Identity _id;
    private String[] _lines;
}
