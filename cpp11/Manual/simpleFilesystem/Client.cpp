// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Filesystem.h>
#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace std;
using namespace Filesystem;

// Recursively print the contents of directory "dir" in tree fashion. 
// For files, show the contents of each file. The "depth"
// parameter is the current nesting level (for indentation).

static void
listRecursive(const shared_ptr<DirectoryPrx>& dir, int depth = 0)
{
    string indent(++depth, '\t');

    NodeSeq contents = dir->list();

    for(const auto& node : contents) {
        auto subdir = Ice::checkedCast<DirectoryPrx>(node);
        cout << indent << node->name() << (subdir ? " (directory):" : " (file):") << endl;
        if(subdir) {
            listRecursive(subdir, depth);
        } else {
            auto file = Ice::uncheckedCast<FilePrx>(node);
            auto text = file->read();
            for(const auto& line : text) {
                cout << indent << "\t" << line << endl;
            }
        }
    }
}

int
main(int argc, char* argv[])
{
    try {
        // Create Ice communicator
        //
        Ice::CommunicatorHolder icHolder = Ice::initialize(argc, argv);

        // Create a proxy for the root directory
        //
        auto base = icHolder->stringToProxy("RootDir:default -p 10000");
        if (!base)
            throw std::runtime_error("Could not create proxy");

        // Down-cast the proxy to a Directory proxy
        //
        auto rootDir = Ice::checkedCast<DirectoryPrx>(base);
        if (!rootDir)
            throw std::runtime_error("Invalid proxy");

        // Recursively list the contents of the root directory
        //
        cout << "Contents of root directory:" << endl;
        listRecursive(rootDir);

    } catch(const std::exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}
