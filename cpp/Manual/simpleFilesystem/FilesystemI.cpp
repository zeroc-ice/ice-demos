// Copyright (c) ZeroC, Inc.

#include <FilesystemI.h>
#include <Ice/Ice.h>

using namespace std;

// Slice Node::name() operation
string
Filesystem::NodeI::name(const Ice::Current&)
{
    return _name;
}

// NodeI constructor
Filesystem::NodeI::NodeI(string name, const shared_ptr<DirectoryI>& parent) : _name(std::move(name)), _parent(parent)
{
    // Create an identity. The root directory has the fixed identity "RootDir"
    _id.name = parent ? Ice::generateUUID() : "RootDir";
}

// NodeI activate() member function
void
Filesystem::NodeI::activate(const shared_ptr<Ice::ObjectAdapter>& adapter)
{
    auto self = Ice::uncheckedCast<NodePrx>(adapter->add(shared_from_this(), _id));
    if (_parent)
    {
        _parent->addChild(self);
    }
}

// Slice File::read() operation
Filesystem::Lines
Filesystem::FileI::read(const Ice::Current&)
{
    return _lines;
}

// Slice File::write() operation
void
Filesystem::FileI::write(Filesystem::Lines text, const Ice::Current&)
{
    _lines = std::move(text);
}

// FileI constructor
Filesystem::FileI::FileI(string name, const shared_ptr<DirectoryI>& parent) : NodeI(std::move(name), parent) {}

// Slice Directory::list() operation
Filesystem::NodeSeq
Filesystem::DirectoryI::list(const Ice::Current&)
{
    return _contents;
}

// DirectoryI constructor
Filesystem::DirectoryI::DirectoryI(string name, const shared_ptr<DirectoryI>& parent) : NodeI(std::move(name), parent)
{
}

// addChild is called by the child in order to add
// itself to the _contents member of the parent
void
Filesystem::DirectoryI::addChild(const optional<NodePrx>& child)
{
    _contents.push_back(child);
}
