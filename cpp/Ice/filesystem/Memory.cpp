// Copyright (c) ZeroC, Inc.

#include "Memory.h"

using namespace std;

//
// MNode
//

Server::MNode::MNode(string name) : _name(std::move(name)) {}

string
Server::MNode::name(const Ice::Current&)
{
    return _name;
}

//
// MFile
//

Filesystem::Lines
Server::MFile::read(const Ice::Current&)
{
    return _lines;
}

void
Server::MFile::write(Filesystem::Lines text, const Ice::Current&)
{
    writeDirect(std::move(text));
}

void
Server::MFile::writeDirect(Filesystem::Lines text)
{
    _lines = std::move(text);
}

//
// MDirectory
//

Filesystem::NodeSeq
Server::MDirectory::list(const Ice::Current&)
{
    return _contents;
}

void
Server::MDirectory::addChild(Filesystem::NodePrx child)
{
    _contents.emplace_back(std::move(child));
}
