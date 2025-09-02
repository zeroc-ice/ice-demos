// Copyright (c) ZeroC, Inc.

#ifndef MEMORY_H
#define MEMORY_H

#include "Inheritance.h"

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4250) // ... : inherits ... via dominance
#endif

// Provides an in-memory implementation of the filesystem objects.
namespace Server
{
    /// Implements Slice interface Node.
    class MNode : public virtual Inheritance::Node
    {
    public:
        /// Constructs a new MNode servant
        /// @param name The name of the node.
        explicit MNode(std::string name);

        // Implements Slice operation name.
        std::string name(const Ice::Current& current) override;

    private:
        std::string _name;
    };

    /// Implements Slice interface File.
    class MFile final : public Inheritance::File, public MNode
    {
    public:
        using MNode::MNode;

        // Implements Slice operation read.
        Inheritance::Lines read(const Ice::Current&) final;

        // Implements Slice operation write.
        void write(Inheritance::Lines text, const Ice::Current&) final;

        /// Writes directly to this file, without going through an Ice operation.
        /// @param text The text to write.
        void writeDirect(Inheritance::Lines text);

    private:
        Inheritance::Lines _lines;
    };

    /// Implements Slice interface Directory.
    class MDirectory final : public Inheritance::Directory, public MNode
    {
    public:
        using MNode::MNode;

        // Implements Slice operation list.
        Inheritance::NodeSeq list(const Ice::Current&) final;

        /// Adds a node to this directory.
        /// @param child The node proxy to add.
        void addChild(Inheritance::NodePrx child);

    private:
        Inheritance::NodeSeq _contents;
    };
}

#endif
