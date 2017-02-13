// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Filesystem.ice>
#include <Ice/Identity.ice>

module FilesystemDB
{
    struct FileEntry
    {
        string name;
        Ice::Identity parent;
        Filesystem::Lines text;
    };

    dictionary<string, Filesystem::NodeDesc> StringNodeDescDict;
    struct DirectoryEntry
    {
        string name;
        Ice::Identity parent;
        StringNodeDescDict nodes;
    };
};
