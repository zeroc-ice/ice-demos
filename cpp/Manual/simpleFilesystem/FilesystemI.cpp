// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <IceUtil/IceUtil.h>
#include <FilesystemI.h>

using namespace std;

// Slice Node::name() operation

std::string
Filesystem::NodeI::name(const Ice::Current&)
{
    return _name;
}

// NodeI constructor

Filesystem::NodeI::NodeI(const string& nm, const DirectoryIPtr& parent) :
    _name(nm), _parent(parent)
{
    // Create an identity. The root directory has the fixed identity "RootDir"
    if(parent)
    {
        _id.name = Ice::generateUUID();
    }
    else
    {
        _id.name = "RootDir";
    }
}

// NodeI activate() member function

void
Filesystem::NodeI::activate(const Ice::ObjectAdapterPtr& a)
{
    NodePrx thisNode = NodePrx::uncheckedCast(a->add(this, _id));
    if(_parent)
    {
        _parent->addChild(thisNode);
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
Filesystem::FileI::write(const Filesystem::Lines& text, const Ice::Current&)
{
    _lines = text;
}

// FileI constructor

Filesystem::FileI::FileI(const string& nm, const DirectoryIPtr& parent) :
    NodeI(nm, parent)
{
}

// Slice Directory::list() operation

Filesystem::NodeSeq
Filesystem::DirectoryI::list(const Ice::Current&)
{
    return _contents;
}

// DirectoryI constructor

Filesystem::DirectoryI::DirectoryI(const string& nm, const DirectoryIPtr& parent) :
    NodeI(nm, parent)
{
}

// addChild is called by the child in order to add
// itself to the _contents member of the parent

void
Filesystem::DirectoryI::addChild(const NodePrx& child)
{
    _contents.push_back(child);
}
