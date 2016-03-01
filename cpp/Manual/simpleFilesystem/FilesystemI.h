// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef __FilesystemI_h__
#define __FilesystemI_h__

#include <Ice/Ice.h>
#include <Filesystem.h>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4250) // ... : inherits ... via dominance
#endif

namespace Filesystem
{
    class DirectoryI;
    typedef IceUtil::Handle<DirectoryI> DirectoryIPtr;

    class NodeI : public virtual Node
    {
    public:

        virtual std::string name(const Ice::Current&);
        NodeI(const std::string&, const DirectoryIPtr&);
        void activate(const Ice::ObjectAdapterPtr&);

    private:

        std::string _name;
        Ice::Identity _id;
        DirectoryIPtr _parent;
        NodeI(const NodeI&);            // Copy forbidden
        void operator=(const NodeI&);   // Assignment forbidden
    };
    typedef IceUtil::Handle<NodeI> NodeIPtr;

    class FileI : public File, public NodeI
    {
    public:

        virtual Lines read(const Ice::Current&);
        virtual void write(const Lines&,
                           const Ice::Current& = Ice::Current());
        FileI(const std::string&, const DirectoryIPtr&);

    private:

        Lines _lines;
    };
    typedef IceUtil::Handle<FileI> FileIPtr;

    class DirectoryI : public Directory, public NodeI
    {
    public:

        virtual NodeSeq list(const Ice::Current&);
        DirectoryI(const std::string&, const DirectoryIPtr&);
        void addChild(const Filesystem::NodePrx&);

    private:

        Filesystem::NodeSeq _contents;
    };
}

#ifdef _MSC_VER
#   pragma warning(pop)
#endif

#endif
