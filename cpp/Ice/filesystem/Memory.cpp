// Copyright (c) ZeroC, Inc.

#include "Memory.h"

using namespace std;
using namespace Memory;

//
// MNode
//

MNode::MNode(string name, MDirectoryPtr parent) : _name(std::move(name)), _parent(std::move(parent))
{
    // Create an identity. The root directory has the fixed identity "RootDir"
    _id.name = _parent ? Ice::generateUUID() : "RootDir";
}

string
MNode::name(const Ice::Current&)
{
    return _name;
}

void
MNode::activate(const Ice::ObjectAdapterPtr& adapter)
{
    auto proxy = adapter->add<Filesystem::NodePrx>(shared_from_this(), _id);
    if (_parent)
    {
        _parent->addChild(proxy);
    }
}

//
// MFile
//

Filesystem::Lines
MFile::read(const Ice::Current&)
{
    return _lines;
}

void
MFile::write(Filesystem::Lines text, const Ice::Current&)
{
    writeDirect(std::move(text));
}

void
MFile::writeDirect(Filesystem::Lines text)
{
    _lines = std::move(text);
}

//
// MDirectory
//

Filesystem::NodeSeq
MDirectory::list(const Ice::Current&)
{
    return _contents;
}

void
MDirectory::addChild(Filesystem::NodePrx child)
{
    _contents.push_back(std::move(child));
}
