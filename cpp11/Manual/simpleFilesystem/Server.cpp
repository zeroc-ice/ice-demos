// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <FilesystemI.h>

using namespace std;
using namespace Filesystem;

class FilesystemApp : public Ice::Application {
public:

    virtual int run(int, char*[])
    {
        //
        // Terminate cleanly on receipt of a signal
        //
        shutdownOnInterrupt();

        //
        // Create an object adapter.
        //
        auto adapter = communicator()->createObjectAdapterWithEndpoints(
            "SimpleFilesystem", "default -h localhost -p 10000");

        //
        // Create the root directory (with name "/" and no parent)
        //
        auto root = make_shared<DirectoryI>("/", nullptr);
        root->activate(adapter);

        //
        // Create a file called "README" in the root directory
        //
        auto file = make_shared<FileI>("README", root);
        auto text = Lines({"This file system contains a collection of poetry."});
        file->write(text);
        file->activate(adapter);

        //
        // Create a directory called "Coleridge" in the root directory
        //
        auto coleridge = make_shared<DirectoryI>("Coleridge", root);
        coleridge->activate(adapter);

        //
        // Create a file called "Kubla_Khan" in the Coleridge directory
        //
        file = make_shared<FileI>("Kubla_Khan", coleridge);
        text = {
            "In Xanadu did Kubla Khan",
            "A stately pleasure-dome decree:",
            "Where Alph, the sacred river, ran",
            "Through caverns measureless to man",
            "Down to a sunless sea."
        };
        file->write(text);
        file->activate(adapter);

        //
        // All objects are created, allow client requests now
        //
        adapter->activate();

        //
        // Wait until we are done
        //
        communicator()->waitForShutdown();
        if(interrupted()) {
            cerr << appName() << ": received signal, shutting down" << endl;
        }

        return 0;
    }
};

int
main(int argc, char* argv[])
{
    FilesystemApp app;
    return app.main(argc, argv);
}
