// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <IceUtil/IceUtil.h>
#include <FilesystemI.h>

using namespace std;

// Slice Node::name() operation
string
Filesystem::NodeI::name(const Ice::Current&)
{
    return _name;
}

// NodeI constructor

Filesystem::NodeI::NodeI(string nm, shared_ptr<DirectoryI> parent) :
    _name(move(nm)),
    _parent(move(parent))
{
    // Create an identity. The root directory has the fixed identity "RootDir"
    if(_parent)
    {
        _id.name = IceUtil::generateUUID();
    }
    else
    {
        _id.name = "RootDir";
    }
}

// NodeI activate() member function

void
Filesystem::NodeI::activate(const Ice::ObjectAdapterPtr& adapter)
{
    auto self = Ice::uncheckedCast<NodePrx>(adapter->add(shared_from_this(), _id));
    if(_parent)
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
    _lines = move(text);
}

// FileI constructor

Filesystem::FileI::FileI(string nm, shared_ptr<DirectoryI> parent) :
    NodeI(move(nm), move(parent))
{
}

// Slice Directory::list() operation

Filesystem::NodeSeq
Filesystem::DirectoryI::list(const Ice::Current&)
{
    return _contents;
}

// DirectoryI constructor

Filesystem::DirectoryI::DirectoryI(string nm, shared_ptr<DirectoryI> parent) :
    NodeI(move(nm), move(parent))
{
}

// addChild is called by the child in order to add
// itself to the _contents member of the parent

void
Filesystem::DirectoryI::addChild(const shared_ptr<NodePrx>& child)
{
    _contents.push_back(child);
}
