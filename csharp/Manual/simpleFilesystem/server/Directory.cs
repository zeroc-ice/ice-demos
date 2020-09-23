// Copyright (c) ZeroC, Inc. All rights reserved.

using System.Collections.Generic;
using ZeroC.Ice;

namespace Filesystem
{
    public class Directory : IDirectory
    {
        private readonly IList<INodePrx> _contents = new List<INodePrx>();
        private readonly string _name;
        private readonly Directory? _parent;

        public Directory(string name, Directory? parent)
        {
            _name = name;
            _parent = parent;
        }

        // Slice Node::name() operation
        public string Name(Current current) => _name;

        // Slice Directory::list() operation
        public IEnumerable<INodePrx> List(Current current) => _contents;

        // AddChild is called by the child in order to add itself to the _contents member of the parent
        public void AddChild(INodePrx child) => _contents.Add(child);

        // Add servant to ASM and parent's _contents map.
        public void Activate(ObjectAdapter adapter)
        {
            if (_parent == null)
            {
                adapter.Add("RootDir", this);
            }
            else
            {
                _parent.AddChild(adapter.Add(System.Guid.NewGuid().ToString(), this, INodePrx.Factory));
            }
        }
    }
}
