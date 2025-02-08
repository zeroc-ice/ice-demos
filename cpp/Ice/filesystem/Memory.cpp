// Copyright (c) ZeroC, Inc.

#include "Memory.h"

using namespace std;
using namespace Memory;

//
// MNode
//

MNode::MNode(string name) : _name(std::move(name)) {}

string
MNode::name(const Ice::Current&)
{
    return _name;
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
