// Copyright (c) ZeroC, Inc. All rights reserved.

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include <iostream>
#include <string>

using namespace std;

int
main(int argc, char* argv[])
{
    try
    {
        // Instantiates DataStorm node.
        DataStorm::Node node(argc, argv, "config.writer");

        // Instantiates the "hello" topic. The topic uses strings for keys and values.
        DataStorm::Topic<string, string> topic(node, "hello");

        // Instantiate writers.
        auto writera = DataStorm::makeSingleKeyWriter(topic, "fooa");
        auto writerb = DataStorm::makeSingleKeyWriter(topic, "foob");
        auto writerc = DataStorm::makeSingleKeyWriter(topic, "fooc");
        auto writerd = DataStorm::makeSingleKeyWriter(topic, "food");
        auto writere = DataStorm::makeSingleKeyWriter(topic, "fooe");

        // Wait for a reader to connect
        topic.waitForReaders();

        // Publish a sample on each writer.
        writera.update("hello");
        writerb.update("hello");
        writerc.update("hello");
        writerd.update("hello");
        writere.update("hello");

        // Wait for readers to disconnect.
        topic.waitForNoReaders();
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
