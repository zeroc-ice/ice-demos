// Copyright (c) ZeroC, Inc.

#ifndef MEMORY_H
#define MEMORY_H

#include "Filesystem.h"

// Provides an in-memory implementation of the Filesystem objects.
namespace Memory
{
    /// Implements Slice interface Node.
    class MNode : public virtual Filesystem::Node
    {
    public:
        /// Constructs a new MNode servant
        explicit MNode(std::string name);

        // Implements Slice operation name.
        std::string name(const Ice::Current& current) override;

    private:
        std::string _name;
    };

    /// Implements Slice interface File.
    class MFile final : public Filesystem::File, public MNode
    {
    public:
        using MNode::MNode;

        // Implements Slice operation read.
        Filesystem::Lines read(const Ice::Current&) final;

        // Implements Slice operation write.
        void write(Filesystem::Lines text, const Ice::Current&) final;

        // Write directly to this file, without going through an Ice operation.
        void writeDirect(Filesystem::Lines text);

    private:
        Filesystem::Lines _lines;
    };

    /// Implements Slice interface Directory.
    class MDirectory final : public Filesystem::Directory, public MNode
    {
    public:
        using MNode::MNode;

        // Implements Slice operation list.
        Filesystem::NodeSeq list(const Ice::Current&) final;

        /// Adds a node to this directory.
        /// @param child The node proxy to add.
        void addChild(Filesystem::NodePrx child);

    private:
        Filesystem::NodeSeq _contents;
    };
}

#endif
