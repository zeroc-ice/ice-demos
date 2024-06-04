// Copyright (c) ZeroC, Inc.

using Filesystem;

internal class DirectoryI : DirectoryDisp_
{
    // DirectoryI constructor
    public DirectoryI(string name, DirectoryI parent)
    {
        _name = name;
        _parent = parent;

        // Create an identity. The root directory has the fixed identity "RootDir"
        _id = new Ice.Identity();
        _id.name = _parent != null ? System.Guid.NewGuid().ToString() : "RootDir";
    }

    // Slice Node::name() operation
    public override string name(Ice.Current current)
    {
        return _name;
    }

    // Slice Directory::list() operation
    public override NodePrx[] list(Ice.Current current)
    {
        return _contents.ToArray();
    }

    // addChild is called by the child in order to add
    // itself to the _contents member of the parent
    public void AddChild(NodePrx child)
    {
        _contents.Add(child);
    }

    // Add servant to ASM and parent's _contents map.
    public void Activate(Ice.ObjectAdapter a)
    {
        var thisNode = NodePrxHelper.uncheckedCast(a.add(this, _id));
        if (_parent != null)
        {
            _parent.AddChild(thisNode);
        }
    }

    private string _name;
    private readonly DirectoryI _parent;
    private Ice.Identity _id;
    private List<NodePrx> _contents = new List<NodePrx>();
}
