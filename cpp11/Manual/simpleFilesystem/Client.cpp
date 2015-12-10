// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Filesystem.h>
#include <iostream>
#include <iterator>

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

    for(const auto& node : contents)
    {
        auto d = Ice::checkedCast<DirectoryPrx>(node);
        cout << indent << node->name() << (d ? " (directory):" : " (file):") << endl;
        if(d)
        {
            listRecursive(d, depth);
        }
        else
        {
            auto file = Ice::uncheckedCast<FilePrx>(node);
            auto text = file->read();
            for(const auto line : text)
            {
                cout << indent << "\t" << line << endl;
            }
        }
    }
}

int
main(int argc, char* argv[])
{
    int status = 0;
    try
    {
        //
        // Create a communicator
        //
        auto ic = Ice::initialize(argc, argv);

        //
        // Create a proxy for the root directory and down-cast the proxy to a Directory proxy
        //
        auto rootDir = Ice::checkedCast<DirectoryPrx>(ic->stringToProxy("RootDir:default -h localhost -p 10000"));
        if(!rootDir)
        {
            throw "Invalid proxy";
        }

        //
        // Recursively list the contents of the root directory
        //
        cout << "Contents of root directory:" << endl;
        listRecursive(rootDir);
    }
    catch(const exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }
    return status;
}
