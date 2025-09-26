// Copyright (c) ZeroC, Inc.

#include <DataStorm/DataStorm.h>

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    try
    {
        // Instantiates DataStorm node.
        DataStorm::Node node{argc, argv};

        // Instantiates the "hello" topic. The topic uses strings for keys and values.
        DataStorm::Topic<string, string> topic{node, "hello"};

        // Instantiate a writer with the key "foo" and wait for a reader to connect.
        auto writer = DataStorm::makeSingleKeyWriter(topic, "foo");
        writer.waitForReaders();

        // Publish a sample.
        writer.update("hello");

        // Wait for the reader to disconnect.
        writer.waitForNoReaders();
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
