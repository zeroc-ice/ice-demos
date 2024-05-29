//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef __FilesystemI_h__
#define __FilesystemI_h__

#include <Filesystem.h>
#include <Ice/Ice.h>

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4250) // ... : inherits ... via dominance
#endif

namespace Filesystem
{

    class DirectoryI;

    class NodeI : public virtual Node, public std::enable_shared_from_this<NodeI>
    {
    public:
        virtual std::string name(const Ice::Current&) override;
        NodeI(std::string, const std::shared_ptr<DirectoryI>&);
        void activate(const std::shared_ptr<Ice::ObjectAdapter>&);

    private:
        std::string _name;
        Ice::Identity _id;
        std::shared_ptr<DirectoryI> _parent;
    };

    class FileI final : public File, public NodeI
    {
    public:
        Lines read(const Ice::Current&) final;
        void write(Lines, const Ice::Current&) final;

        FileI(std::string, const std::shared_ptr<DirectoryI>&);

    private:
        Lines _lines;
    };

    class DirectoryI : public Directory, public NodeI
    {
    public:
        NodeSeq list(const Ice::Current&) final;
        DirectoryI(std::string, const std::shared_ptr<DirectoryI>&);
        void addChild(const std::optional<NodePrx>&);

    private:
        NodeSeq _contents;
    };
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

#endif
