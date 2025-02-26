// Copyright (c) ZeroC, Inc.

#include "Filesystem.h"

#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Filesystem;

/// Recursively print the contents of a directory in tree fashion. For files, show the contents of each file.
/// @param dir The directory to list.
/// @param depth The current nesting level (for indentation).
static void
listRecursive(const DirectoryPrx& dir, size_t depth = 0)
{
    const string indent(++depth, '\t');

    // The node proxies returned by list() are never null.
    NodeSeq contents = dir->list();

    for (const optional<NodePrx>& node : contents)
    {
        auto subdir = Ice::checkedCast<DirectoryPrx>(node);
        cout << indent << node->name() << (subdir ? " (directory):" : " (file):") << endl;
        if (subdir)
        {
            listRecursive(*subdir, depth);
        }
        else
        {
            auto file = Ice::uncheckedCast<FilePrx>(node);
            Lines lines = file->read();
            for (const auto& line : lines)
            {
                cout << indent << '\t' << line << endl;
            }
        }
    }
}

int
main(int argc, char* argv[])
{
    // Create an Ice communicator to initialize the Ice runtime.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create a proxy for the root directory.
    DirectoryPrx rootDir{communicator, "RootDir:tcp -h localhost -p 4061"};

    // Recursively list the contents of the root directory.
    cout << "Contents of root directory:" << endl;
    listRecursive(rootDir);

    return 0;
}
