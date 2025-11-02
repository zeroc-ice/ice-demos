// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"

#include <DataStorm/DataStorm.h>

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create a DataStorm node using the provided command-line arguments.
    DataStorm::Node node{argc, argv};

    // Create a topic named "greeter" with string keys and string values.
    DataStorm::Topic<string, string> topic{node, "greeter"};

    // Create a writer for a single key using the current username as the key.
    auto writer = DataStorm::makeSingleKeyWriter(topic, Env::getUsername());

    // Wait until at least one reader is connected.
    writer.waitForReaders();

    // Publish a sample with the value "Hello!".
    writer.update("Hello!");

    // Wait until all readers have disconnected.
    writer.waitForNoReaders();
    return 0;
}
