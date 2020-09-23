// Copyright (c) ZeroC, Inc. All rights reserved.

using System.Collections.Generic;
using ZeroC.Ice;

namespace Filesystem
{
    public class File : IFile
    {
        private string[] _lines;
        private readonly string _name;
        private readonly Directory _parent;

        public File(string name, Directory parent, string[] lines)
        {
            _name = name;
            _parent = parent;
            _lines = lines;
        }

        // Slice Node::name() operation
        public string Name(Current current) => _name;

        // Slice File::read() operation
        public IEnumerable<string> Read(Current current) => _lines;

        // Slice File::write() operation
        public void Write(string[] text, Current current) => _lines = text;

        // Add servant to ASM and parent's _contents map.
        public void Activate(ObjectAdapter adapter) =>
            _parent.AddChild(adapter.Add(System.Guid.NewGuid().ToString(), this, INodePrx.Factory));
    }

}
