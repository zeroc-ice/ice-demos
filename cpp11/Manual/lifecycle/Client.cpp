// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Filesystem.h>
#include <Parser.h>

using namespace std;
using namespace Filesystem;

int
main(int argc, char* argv[])
{
    int status = EXIT_SUCCESS;

    try
    {
        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv);

        //
        // Create a proxy for the root directory
        //
        auto base = ich->stringToProxy("RootDir:default -p 10000");

        //
        // Down-cast the proxy to a Directory proxy.
        //
        auto rootDir = Ice::checkedCast<DirectoryPrx>(base);
        if(!rootDir)
        {
            cerr << "Invalid proxy" << endl;
            status = EXIT_FAILURE;
        }
        else
        {
            unique_ptr<Parser> p(new Parser(rootDir));
            status = p->parse();
        }
    }
    catch(const std::exception& ex)
    {
        cerr << argv[0] << ": " << ex.what() << endl;
        status = EXIT_FAILURE;
    }

    return status;
}
