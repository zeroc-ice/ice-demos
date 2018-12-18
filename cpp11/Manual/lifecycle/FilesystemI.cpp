// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <FilesystemI.h>

using namespace std;
using namespace Filesystem;
using namespace FilesystemI;

// Slice Node::name() operation.

std::string
FilesystemI::NodeI::name(const Ice::Current& c)
{
    lock_guard<mutex> lock(_mutex);

    if(_destroyed)
    {
        throw Ice::ObjectNotExistException(__FILE__, __LINE__, c.id, c.facet, c.operation);
    }

    return _name;
}

// Return the object identity for this node.

Ice::Identity
FilesystemI::NodeI::id() const
{
    return _id;
}

// NodeI constructor.

FilesystemI::NodeI::NodeI(const string& nm, const shared_ptr<DirectoryI>& parent)
    : _name(nm), _parent(parent), _destroyed(false)
{
    //
    // Create an identity. The root directory has the fixed identity "RootDir".
    //
    if(parent != nullptr)
    {
        _id.name = Ice::generateUUID();
    }
    else
    {
        _id.name = "RootDir";
    }
}

// Slice File::read() operation.

Lines
FilesystemI::FileI::read(const Ice::Current& c)
{
    lock_guard<mutex> lock(_mutex);

    if(_destroyed)
    {
        throw Ice::ObjectNotExistException(__FILE__, __LINE__, c.id, c.facet, c.operation);
    }

    return _lines;
}

// Slice File::write() operation.

void
FilesystemI::FileI::write(Lines text, const Ice::Current& c)
{
    lock_guard<mutex> lock(_mutex);

    if(_destroyed)
    {
        throw Ice::ObjectNotExistException(__FILE__, __LINE__, c.id, c.facet, c.operation);
    }

    _lines = move(text);
}

// Slice File::destroy() operation.

void
FilesystemI::FileI::destroy(const Ice::Current& c)
{
    {
        lock_guard<mutex> lock(_mutex);

        if(_destroyed)
        {
            throw Ice::ObjectNotExistException(__FILE__, __LINE__, c.id, c.facet, c.operation);
        }

        c.adapter->remove(id());
        _destroyed = true;
    }

    _parent->removeEntry(_name);
}

// FileI constructor.

FilesystemI::FileI::FileI(const string& nm, const shared_ptr<DirectoryI>& parent)
    : NodeI(nm, parent)
{
}

// Slice Directory::list() operation.

NodeDescSeq
FilesystemI::DirectoryI::list(const Ice::Current& c)
{
    lock_guard<mutex> lock(_mutex);

    if(_destroyed)
    {
        throw Ice::ObjectNotExistException(__FILE__, __LINE__, c.id, c.facet, c.operation);
    }

    NodeDescSeq ret;
    for(const auto& i: _contents)
    {
        NodeDesc d;
        d.name = i.first;
        d.type = dynamic_pointer_cast<File>(i.second) ? NodeType::FileType : NodeType::DirType;
        d.proxy = Ice::uncheckedCast<NodePrx>(c.adapter->createProxy(i.second->id()));
        ret.push_back(d);
    }
    return ret;
}

// Slice Directory::find() operation.

NodeDesc
FilesystemI::DirectoryI::find(string nm, const Ice::Current& c)
{
    lock_guard<mutex> lock(_mutex);

    if(_destroyed)
    {
        throw Ice::ObjectNotExistException(__FILE__, __LINE__, c.id, c.facet, c.operation);
    }

    auto pos = _contents.find(nm);
    if(pos == _contents.end())
    {
        throw NoSuchName(nm);
    }

    auto p = pos->second;
    NodeDesc d;
    d.name = nm;
    d.type = dynamic_pointer_cast<File>(p) ? NodeType::FileType : NodeType::DirType;
    d.proxy = Ice::uncheckedCast<NodePrx>(c.adapter->createProxy(p->id()));
    return d;
}

// Slice Directory::createFile() operation.

shared_ptr<FilePrx>
FilesystemI::DirectoryI::createFile(string nm, const Ice::Current& c)
{
    lock_guard<mutex> lock(_mutex);

    if(_destroyed)
    {
        throw Ice::ObjectNotExistException(__FILE__, __LINE__, c.id, c.facet, c.operation);
    }

    if(nm.empty() || _contents.find(nm) != _contents.end())
    {
        throw NameInUse(nm);
    }

    auto f = make_shared<FileI>(nm, shared_from_this());
    auto node = c.adapter->add(f, f->id());
    _contents[nm] = f;
    return Ice::uncheckedCast<FilePrx>(node);
}

// Slice Directory::createDirectory() operation.

shared_ptr<DirectoryPrx>
FilesystemI::DirectoryI::createDirectory(string nm, const Ice::Current& c)
{
    lock_guard<mutex> lock(_mutex);

    if(_destroyed)
    {
        throw Ice::ObjectNotExistException(__FILE__, __LINE__, c.id, c.facet, c.operation);
    }

    if(nm.empty() || _contents.find(nm) != _contents.end())
    {
        throw NameInUse(nm);
    }

    auto d = make_shared<DirectoryI>(nm, shared_from_this());
    auto node = c.adapter->add(d, d->id());
    _contents[nm] = d;
    return Ice::uncheckedCast<DirectoryPrx>(node);
}

// Slice Directory::destroy() operation.

void
FilesystemI::DirectoryI::destroy(const Ice::Current& c)
{
    if(!_parent)
    {
        throw PermissionDenied("Cannot destroy root directory");
    }

    {
        lock_guard<mutex> lock(_mutex);

        if(_destroyed)
        {
            throw Ice::ObjectNotExistException(__FILE__, __LINE__, c.id, c.facet, c.operation);
        }

        if(!_contents.empty())
        {
            throw PermissionDenied("Cannot destroy non-empty directory");
        }

        c.adapter->remove(id());
        _destroyed = true;
    }

    _parent->removeEntry(_name);
}

// DirectoryI constructor.

FilesystemI::DirectoryI::DirectoryI(const string& nm, const shared_ptr<DirectoryI>& parent) :
    NodeI(nm, parent)
{
}

// Remove the entry from the _contents map.

void
FilesystemI::DirectoryI::removeEntry(const string& nm)
{
    lock_guard<mutex> lock(_mutex);
    auto i = _contents.find(nm);
    if(i != _contents.end())
    {
        _contents.erase(i);
    }
}
