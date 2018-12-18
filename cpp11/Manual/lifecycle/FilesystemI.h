// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef __FilesystemI_h__
#define __FilesystemI_h__

#include <Ice/Ice.h>
#include <Filesystem.h>
#include <map>
#include <mutex>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4250) // ... : inherits ... via dominance
#endif

namespace FilesystemI
{

class DirectoryI;

class NodeI : public virtual Filesystem::Node
{
public:

    virtual std::string name(const Ice::Current&) override;
    Ice::Identity id() const;

protected:

    NodeI(const std::string&, const std::shared_ptr<DirectoryI>&);

    const std::string _name;
    const std::shared_ptr<DirectoryI> _parent;
    bool _destroyed;
    Ice::Identity _id;
    std::mutex _mutex;
};

class FileI : public Filesystem::File, public NodeI
{
public:

    virtual Filesystem::Lines read(const Ice::Current&) override;
    virtual void write(Filesystem::Lines, const Ice::Current&) override;
    virtual void destroy(const Ice::Current&) override;

    FileI(const std::string&, const std::shared_ptr<DirectoryI>&);

private:

    Filesystem::Lines _lines;
};

class DirectoryI : public NodeI, public Filesystem::Directory, public std::enable_shared_from_this<DirectoryI>
{
public:

    virtual Filesystem::NodeDescSeq list(const Ice::Current&) override;
    virtual Filesystem::NodeDesc find(std::string, const Ice::Current&) override;
    std::shared_ptr<Filesystem::FilePrx> createFile(std::string, const Ice::Current&) override;
    std::shared_ptr<Filesystem::DirectoryPrx> createDirectory(std::string, const Ice::Current&) override;
    virtual void destroy(const Ice::Current&) override;

    DirectoryI(const std::string& = "/", const std::shared_ptr<DirectoryI>& = nullptr);

    void removeEntry(const std::string&);

private:

    using Contents = std::map<std::string, std::shared_ptr<NodeI>>;
    Contents _contents;
};
}

#ifdef _MSC_VER
#   pragma warning(pop)
#endif

#endif
