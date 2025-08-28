// Copyright (c) ZeroC, Inc.

#pragma once

module Filesystem
{
    /// Represents a {@link File::write} failure.
    exception WriteException
    {
        /// The reason for the write failure.
        ["cs:identifier:Reason"]
        string reason;
    }

    /// The base interface for files and directories.
    interface Node
    {
        /// Gets the name of this node.
        /// @return The name of the node.
        ["cs:identifier:Name"] // remap the C# identifier to Name instead of the default (name).
        idempotent string name();
    }

    /// The contents of a file.
    sequence<string> Lines;

    /// Represents a file in our filesystem. A file holds lines of text.
    interface File extends Node
    {
        /// Reads the file and returns its contents.
        /// @return The contents of the file.
        ["cs:identifier:Read"]
        idempotent Lines read();

        /// Overwrites the file with new contents.
        /// @param text The new contents of the file.
        /// @throws WriteException Thrown if the file cannot be written to.
        ["cs:identifier:Write"]
        idempotent void write(Lines text) throws WriteException;
    }

    /// A list of node proxies.
    sequence<Node*> NodeSeq;

    /// Represents a directory. A directory holds files and other directories.
    interface Directory extends Node
    {
        /// Gets the contents of the directory.
        /// @return The contents of the directory, as a list of node proxies.
        ["cs:identifier:List"]
        idempotent NodeSeq list();
    }
}
